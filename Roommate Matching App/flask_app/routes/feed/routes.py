"""
Endpoints used for the feed.
"""
from flask import Blueprint, request, jsonify, session
from database import db, User, Like, Question, Answer, Category, User_Photo
from datetime import date
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.metrics.pairwise import cosine_similarity
import numpy as np
from test_data import age_similarity_weight, bio_similarity_weight, major_similarity_weight, gender_similarity_weight, answer_similarity_weight
from sqlalchemy import and_, distinct, or_
from sqlalchemy.orm import aliased
import os
from run import s3,s3_bucket

feed_bp = Blueprint('feed', __name__)

@feed_bp.route('/like', methods=['POST'])
def like():
    """
    Endpoint to like another user.

    :param int liked_user_id: The ID of the user being liked.

    :return: Response
        A JSON response indicating whether the like was successful and if it resulted in a match.
        - message (str): A message indicating the result of the like operation.
        - match (bool): Indicates whether the like resulted in a match.

    :raises ValueError: If the request does not contain JSON data.
    :raises ValueError: If either the user ID or the liked user ID is missing from the request.
    """
    data = request.json
    
    if not data:
        return jsonify({'error': 'No data provided'}), 400
    
    user_id = session.get('user_id')
    liked_user_id = data.get('liked_user_id')
    
    if not user_id or not liked_user_id:
        return jsonify({'error': 'Missing user ID or liked user ID'}), 400

    like = Like(user_id=user_id, liked_user_id=liked_user_id, liked=True)
    db.session.add(like)
    db.session.commit()

    if check_for_match(user_id, liked_user_id):
        return jsonify({'message': 'Like successful', 'match' : True}), 200
    else:
        return jsonify({'message': 'Like successful', 'match' : False}), 200
    
    
@feed_bp.route('/dislike', methods=['POST'])
def dislike():
    """
    Endpoint to dislike another user.

    :param int liked_user_id: The ID of the user being disliked.

    :return: Response
        A JSON response indicating that the dislike was successful.
        - message (str): A message indicating the result of the dislike operation.

    :raises ValueError: If the request does not contain JSON data.
    :raises ValueError: If either the user ID or the liked user ID is missing from the request.
    """
    data = request.json
    
    if not data:
        return jsonify({'error': 'No data provided'}), 400
    
    user_id = session.get('user_id')
    liked_user_id = data.get('liked_user_id')
    
    if not user_id or not liked_user_id:
        return jsonify({'error': 'Missing user ID or liked user ID'}), 400

    like = Like(user_id=user_id, liked_user_id=liked_user_id, liked=False)
    db.session.add(like)
    db.session.commit()
    
    return jsonify({'message': 'Dilike successful'}), 200


    

@feed_bp.route('/getfeed', methods=['GET'])
def getfeed():
    """
    Endpoint to fetch the feed of potential matches for the current user.

    :return: Response
        A JSON response containing a list of user IDs representing potential matches.

    :raises ValueError: If the current user is not authenticated.
    """
    current_user_id = session.get('user_id')

    if current_user_id is None:
        return jsonify({'error': 'User not authenticated'}), 401
    
    current_user = User.query.filter_by(user_id=current_user_id).first()

    #TODO MAYBE add a limit to the number of users fetched at a time?????
    #get all users not liked/disliked by current user, exclude users with incomplete profiles 
    users = User.query.filter(User.user_id != current_user_id) \
                  .filter(~User.user_id.in_(
                      Like.query.filter_by(user_id=current_user_id).with_entities(Like.liked_user_id)
                  )) \
                  .filter(and_(
                      User.bio.isnot(None),
                      User.birthday.isnot(None),
                      User.major.isnot(None),
                      User.first_name.isnot(None),
                      User.profile_photo.isnot(None),
                      User.gender.isnot(None),
                      User.deactivated.isnot(True)
                  )).all()
    
    if users:
        #call algo function
        user_similarities = compare_users_to_current(current_user, users)

        #sort based on how similar they are (descending)
        sorted_user_similarities = sorted(user_similarities, key=lambda x: x[1], reverse=True)

        user_ids = [ user_id for user_id,similarity_score  in sorted_user_similarities]
    else:
        user_ids = []

    return jsonify({'user_ids': user_ids}), 200


@feed_bp.route('/unmatch/<int:matched_user_id>', methods=['POST'])
def unmatch(matched_user_id):
    """
    Endpoint to unmatch with a previously matched user.

    :param int matched_user_id: The ID of the user to unmatch.

    :return: Response
        A JSON response indicating whether the unmatch was successful.
        - message (str): A message indicating the result of the unmatch operation.

    :raises ValueError: If the method is not POST.
    :raises ValueError: If no matching entry is found for the current user and the matched user.
    """
    if request.method == 'POST':
        current_user_id = session.get('user_id')
        like_entry = Like.query.filter_by(user_id=current_user_id, liked_user_id=matched_user_id).first()
        if like_entry:
            like_entry.liked = False
            db.session.commit()
            return jsonify({'message': 'Unmatched successfully'}), 200
        else:
            return jsonify({'error': 'No matching entry found'}), 404
    else:
        return jsonify({'error': 'Method not allowed'}), 405


@feed_bp.route('/userinfo', methods=['GET'])
def userinfo():
    """
    Endpoint to retrieve user information.

    :param int user_id: The ID of the user whose information is requested.

    :return: Response
        A JSON response containing the user's information.
        - first_name (str): The user's first name.
        - age (int): The user's age.
        - major (str): The user's major.
        - bio (str): The user's biography.
        - categories (dict): A dictionary containing categories and their corresponding answered questions.
        - photos (list): A list of URLs to the user's photos.
        - gender (str): The user's gender.
        - verified (bool): Whether the user is verified.
        - email (str): The user's email.

    :raises ValueError: If no user ID is provided or if the user is not found.
    """
    user_id = request.args.get("user_id")
    if user_id is None:
        return jsonify({'error': 'No user id provided'}), 400

    user = User.query.get(user_id)
    if user is None:
        return jsonify({'error': 'User not found'}), 404
    
    photos = User_Photo.query.filter_by(user_id=user.user_id).all()
    photo_urls = [photo.photo_url for photo in photos]

    if user.profile_photo:
        photo_urls.insert(0, user.profile_photo)

    for index, photo_url in enumerate(photo_urls):
        #generate presigned link for photos in aws bucket
        if os.getenv('FLASK_ENV') == 'production':
            if photo_url.startswith('aggies_house_'):
                s3_link = s3.generate_presigned_url(
                    'get_object',
                    Params={'Bucket': s3_bucket, 'Key': photo_url},
                    ExpiresIn=300  # URL expires in 1 hour
                )
                photo_urls[index] = s3_link
        
    user_info = {
            'first_name': user.first_name,
            'age': calculate_age(user.birthday),
            'major': user.major,
            'bio': user.bio,
            'categories': {},
            'photos': photo_urls,
            'gender': user.gender,
            'verified': user.verified if user.verified else None,
            'email': user.email
        }

    categories = Category.query.all()

    for category in categories:
        category_data = {}
        questions = Question.query.filter_by(category_id=category.category_id).all()
        for question in questions:
            answers = Answer.query.filter_by(user_id=user.user_id, question_id=question.question_id).all()

            if len(answers) > 1:
                answer_text = [answer.answer_text for answer in answers]
                category_data[question.question_text] = answer_text
            elif answers and answers[0].answer_text != None:
                category_data[question.question_text] = answers[0].answer_text
        # Only add categories that have at least one question answered by the user
        if any(category_data.values()):
            user_info['categories'][category.category_name] = category_data

    return user_info


def calculate_age(birthday):
    """
    Calculate age based on the provided birthday.

    :param date birthday: The date of birth of the user.

    :return: int or None
        The calculated age of the user, or None if the birthday is not provided.
    """
     
    if birthday is None:
        return None

    today = date.today()
    age = today.year - birthday.year - ((today.month, today.day) < (birthday.month, birthday.day))

    return age


def check_for_match(user_id_1, user_id_2):
    """
    Check if two users have mutually liked each other.

    :param int user_id_1: The ID of the first user.
    :param int user_id_2: The ID of the second user.

    :return: bool
        True if both users have liked each other, False otherwise.
    """
    liked_1_to_2 = Like.query.filter_by(user_id=user_id_1, liked_user_id=user_id_2, liked=True).first()
    liked_2_to_1 = Like.query.filter_by(user_id=user_id_2, liked_user_id=user_id_1, liked=True).first()
    
    # Check if both users liked each other
    if liked_1_to_2 and liked_2_to_1:
        return True
    else:
        return False
    
def compare_users_to_current(current_user, users):
    """
    Compare current user to other users based on various attributes.

    :param User current_user: The current user for comparison.
    :param list users: List of User objects to compare against.
    :param float age_similarity_weight: Weight for age similarity comparison.
    :param float bio_similarity_weight: Weight for biography similarity comparison.
    :param float major_similarity_weight: Weight for major similarity comparison.
    :param float gender_similarity_weight: Weight for gender similarity comparison.
    :param float answer_similarity_weight: Weight for answer similarity comparison.

    :return: list
        A list of tuples containing user IDs and their overall similarity scores compared to the current user.
    """

    similarities = []

    for user in users:
        #compare bios
        tfidf_vectorizer = TfidfVectorizer(stop_words='english')
        bios = [current_user.bio, user.bio]
        tfidf_matrix = tfidf_vectorizer.fit_transform(bios)
        bio_similarity = cosine_similarity(tfidf_matrix[0:1], tfidf_matrix[1:2])[0][0]
        
        #compare age
        age_similarity = 1 - ((abs(calculate_age(current_user.birthday) - calculate_age(user.birthday)) ** 2) / 100)
        
        #compare majors - dont know if i need to do this even? simple comparison may work
        major_similarity = 1 if current_user.major == user.major else 0

        #compare genders - same as above ^^ probably dont need
        gender_similarity = 1 if current_user.gender == user.gender else 0
        

        #get questions and the answers for the questions both users have answered - for only multiple choice
        AnswerAlias = aliased(Answer)

        common_question_answers = db.session.query(
                                        Question.question_text,
                                        Answer.answer_text.label('current_user_answer_text'),
                                        AnswerAlias.answer_text.label('other_user_answer_text'),
                                    )\
                                    .join(Question, Question.question_id == Answer.question_id)\
                                    .join(AnswerAlias, Answer.question_id == AnswerAlias.question_id)\
                                    .filter(and_(
                                        Answer.user_id == current_user.user_id,
                                        AnswerAlias.user_id == user.user_id,
                                        Answer.question_id.in_(
                                            db.session.query(distinct(Answer.question_id))
                                            .filter(Answer.user_id == user.user_id)
                                        ),
                                        Question.question_type == 'multiple choice'
                                    ))\
                                    .all()

        #compare answers to questions
        answer_similarities = []

        # Iterate through each question and its answers
        for question_text, user1_answer, user2_answer in common_question_answers:
            if user1_answer is not None and user2_answer is not None:
                similarity = 1 if user1_answer == user2_answer else 0
                answer_similarities.append(similarity)

        # Filter common questions and answers for multi select questions
        common_question_ids = db.session.query(Answer.question_id, Question.question_text)\
                            .join(Question, Question.question_id == Answer.question_id)\
                            .filter(Answer.user_id == current_user.user_id)\
                            .filter(Answer.question_id.in_(
                                db.session.query(Answer.question_id)
                                .join(Question, Question.question_id == Answer.question_id)
                                .filter(Answer.user_id == user.user_id)
                                .filter(Question.question_type == 'multi select')
                            )).distinct().all()

        # Get each user's answeers for the multi select questions they have in common and compare the two
        for question_id, question_text in common_question_ids:
            user1_answers = db.session.query(Answer.answer_text)\
                            .filter(Answer.user_id == current_user.user_id)\
                            .filter(Answer.question_id == question_id).all()
            user1_multi_select_answers = set([answer for (answer,) in user1_answers])

            user2_answers = db.session.query(Answer.answer_text)\
                            .filter(Answer.user_id == user.user_id)\
                            .filter(Answer.question_id == question_id).all()
            user2_multi_select_answers = set([answer for (answer,) in user2_answers])
            
            # Calculate overlap coefficient between the two sets (size of intersection/size of smallest set)
            intersection = len(user1_multi_select_answers.intersection(user2_multi_select_answers))
            min_size = min(len(user1_multi_select_answers), len(user2_multi_select_answers))
            overlap_coefficient = intersection / min_size if min_size != 0 else 0
            answer_similarities.append(overlap_coefficient)


        # Calculate the average similarity across all answered questions
        answer_similarity = sum(answer_similarities) / len(answer_similarities) if answer_similarities else 0.0

        # Calculate overall similarity (simple mean, could add weights later on)
        overall_similarity = age_similarity*age_similarity_weight +  bio_similarity*bio_similarity_weight +  major_similarity*major_similarity_weight + gender_similarity*gender_similarity_weight + answer_similarity*answer_similarity_weight
        
        similarities.append((user.user_id, overall_similarity))
    
    return similarities
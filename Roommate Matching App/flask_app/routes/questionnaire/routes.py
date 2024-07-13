"""
Endpoints used for the questionnaire.
"""
from flask import Blueprint, request, jsonify, session, current_app
from database import db, Question, Answer, Category, User, Answer_Choice
from sqlalchemy import text, func
# from flask_mail import Message
# from flask_mail import Mail
# from run import mail
import secrets
from datetime import datetime
import os
import requests
import uuid

question_bp = Blueprint('questionnaire', __name__)


@question_bp.route('/questionnaire/<int:category_id>', methods=['GET'])
def get_questions_by_category(category_id):
    """
    Get questions by category ID.

    :param int category_id: The ID of the category.

    :return: Response
        A JSON response containing questions and their answer choices.
    """
    questions = Question.query.filter_by(category_id=category_id).all()

    #get answer choices for each question that has answer choices
    answer_choices_by_question = {}
    answer_choices = db.session.query(Answer_Choice.choice_text, Question.question_id, Question.question_text)\
    .join(Question, Question.question_id == Answer_Choice.question_id)\
    .order_by(Question.question_id)\
    .all()

    for choice_text, question_id, question_text in answer_choices:
        if question_id not in answer_choices_by_question:
            answer_choices_by_question[question_id] = {'question_text': question_text, 'choices': []}
        answer_choices_by_question[question_id]['choices'].append(choice_text)

    questions_list = []
    for question in questions:
        question_dict = question.serialize()
        if question.question_id in answer_choices_by_question:
            question_dict['options'] = answer_choices_by_question[question.question_id]['choices']
        questions_list.append(question_dict)

    return jsonify(questions_list)


@question_bp.route('/save_answers', methods=['POST'])
def save_answers():
    """
    Endpoint to save user answers to questions.

    :return: Response
        A JSON response indicating the success of saving the answers.
    """
    data = request.json
    user_id = session.get('user_id')  # Retrieve user ID from session
    answers = data.get('answers')

    if user_id is None:
        return jsonify({'error': 'User not authenticated'}), 401

    for question_id, answer_text in answers.items():
        existing_answer = Answer.query.filter_by(user_id=user_id, question_id=int(question_id)).first()

        if existing_answer and answer_text:
            # If the key already exists, override the existing answer
            if not isinstance(answer_text,list):
                existing_answer.answer_text = answer_text
            else:
                #for multi select delete all exisiting answers and add newly selected answers 
                Answer.query.filter_by(user_id=user_id, question_id=question_id).delete()
                db.session.commit()
                for answer in answer_text:
                    answer = Answer(user_id=user_id, question_id=question_id, answer_text=answer)
                    db.session.add(answer)
        elif answer_text == None:
            Answer.query.filter_by(user_id=user_id, question_id=question_id).delete()
            db.session.commit()
        else:
            if isinstance(answer_text, list):  # Check if answer is a list (indicating multiple selections)
                for answer in answer_text:
                    answer = Answer(user_id=user_id, question_id=int(question_id), answer_text=answer)
                    db.session.add(answer)
            else:
                answer = Answer(user_id=user_id, question_id=int(question_id), answer_text=answer_text)
                db.session.add(answer)

    db.session.commit()

    return jsonify({'message': 'Answers saved successfully'})



@question_bp.route('/get_user_id', methods=['GET'])
def get_user_id():
    """
    Endpoint to retrieve the user ID from the session.

    :return: Response
        A JSON response containing the user ID if found, or an error message if not found.
    """
    user_id =session.get('user_id')
    if user_id is not None:
        return jsonify({'user_id' : user_id}),200
    else:
        return jsonify({'error': 'User ID not found'}), 404
    

@question_bp.route('/user_answers/<int:category_id>', methods=['GET'])
def get_user_answers_by_category(category_id):
    """
    Endpoint to retrieve user answers by category ID.

    :param int category_id: The ID of the category.

    :return: Response
        A JSON response containing user answers for questions in the specified category.
    """
    user_id = session.get('user_id')

    if user_id is None:
        return jsonify({'error': 'User not authenticated'}), 401

    # Fetch user answers for questions in the specified category, answers are returned in list format
    user_answers = (
        db.session.query(Question.question_id, func.array_agg(Answer.answer_text))
        .join(Answer)
        .filter(Answer.user_id == user_id, Question.category_id == category_id)
        .group_by(Question.question_id)
        .all()
    )

    user_answers_dict = dict(user_answers)

    return jsonify(user_answers_dict)

@question_bp.route('/profile_info', methods=['GET'])
def get_profile_info():
    """
    Endpoint to retrieve profile information of the logged-in user.

    :return: Response
        A JSON response containing the user's profile information.
    """
    user_id =session.get('user_id')
    if user_id is None:
        return jsonify({'error': 'User not logged in'}), 400

    user = User.query.get(user_id)
    if user is None:
        return jsonify({'error': 'User not found'}), 404
    
    profile_info = {
        'first_name': user.first_name, 
        'birthday': user.birthday.strftime('%Y-%m-%d') if user.birthday else None, 
        'gender': user.gender, 
        'major': user.major, 
        'bio': user.bio
    }
    return jsonify(profile_info), 200

@question_bp.route('/profile_info', methods=['POST'])
def update_profile_info():
    """
    Endpoint to update the profile information of the logged-in user.

    :return: Response
        A JSON response indicating the success or failure of the profile update.
    """
    user_id = session.get('user_id')
    if user_id is None:
        return jsonify({'error': 'User not logged in'}), 400

    user = User.query.get(user_id)
    if user is None:
        return jsonify({'error': 'User not found'}), 404

    data = request.json
    user.first_name = data.get('first_name', user.first_name)
    user.birthday = datetime.strptime(data.get('birthday'), '%Y-%m-%d') if data.get('birthday') else user.birthday
    user.gender = data.get('gender', user.gender)
    user.major = data.get('major', user.major)
    user.bio = data.get('bio', user.bio)
    password = user.password

    if user.password == None:
        password = uuid.uuid4().hex
        user.password = password

    db.session.commit()

        #chat engine
    # Make an API call to Chat Engine for user registration
    chat_engine_url = 'https://api.chatengine.io/users/'
    chat_engine_private_key = os.getenv('CHAT_ENGINE_PRIVATE_KEY')  # Replace with your Chat Engine private key
    headers = {'PRIVATE-KEY': chat_engine_private_key}
    chat_engine_data = {
        "username": user.email,  # Using email as username for simplicity, you can adjust this as needed
        "secret": password,  # Using password as secret for simplicity, ensure it's secure in production
        "email": user.email,
        "first_name": user.first_name,
        # Add other fields as needed
    }

    try:
        response = requests.post(chat_engine_url, headers=headers, json=chat_engine_data)
        response.raise_for_status()  # Raise exception if request fails
    except requests.exceptions.RequestException as e:
        print("here chat")
    
    return jsonify({'message': 'Profile info updated successfully'}), 200

# def send_verification_email(user):
#     # token = generate_verification_token()
#     # user.verification_token = token
#     # db.session.commit()

#     # verification_link = url_for('questionnaire.verify_email', token=token, _external=True)
#     message = Message("hey", sender='aggieshouse@fastmail.com',
#                     recipients=[user.tamuEmail])
#     message.body = "test"
#     # message.body = render_template('verification_email.txt', verification_link=verification_link)
#     # message.html = render_template('verification_email.html', verification_link=verification_link)
#     mail.send(message)


# @question_bp.route('/verify_email/<token>')
# def verify_email(token):
#     user = User.query.filter_by(verification_token=token).first_or_404()

#     if user:
#         user.verified = True
#         user.verification_token = None
#         db.session.commit()
#         return 'Email verified successfully.'
#     else:
#         return 'Invalid token.'

# def generate_verification_token():
#     return secrets.token_urlsafe(30)

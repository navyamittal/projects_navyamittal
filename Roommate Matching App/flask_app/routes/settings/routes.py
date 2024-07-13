"""
Endpoints used for the edit profile page.
"""
from flask import Blueprint, request, jsonify, session
from database import db, User, Like, Question, Answer, Category, User_Photo
from datetime import date


settings_bp = Blueprint('settings', __name__, url_prefix='/settings')

@settings_bp.route('/get_category_id', methods=['GET'])
def get_category_id():
    """
    Endpoint to retrieve the category ID based on the provided category name.

    :return: Response
        A JSON response containing the category ID if found, or an error message if not found.
    """
    category_name = request.args.get("category")
    
    category = Category.query.filter_by(category_name = category_name).first()

    if category is not None:
        return jsonify({'category_id' : category.category_id}),200
    else:
        return jsonify({'error': 'Category ID not found'}), 404

@settings_bp.route('/edit_answers', methods=['POST'])
def edit_answers():
    """
    Endpoint to edit user answers for questions.

    :param dict request.json: A JSON object containing the updated answers keyed by question IDs.

    :return: Response
        A JSON response indicating the success or failure of the operation.
    """
    data = request.json
    user_id = session.get('user_id')  # Retrieve user ID from session
    answers = data.get('answers')

    if user_id is None:
        return jsonify({'error': 'User not authenticated'}), 401

    for question_id, answer_text in answers.items():
        existing_answer = Answer.query.filter_by(user_id=user_id, question_id=int(question_id)).first()
        if existing_answer and answer_text != None:
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

    return jsonify({'message': 'Answers updated successfully'})


@settings_bp.route('/get_empty_categories', methods=['GET'])
def get_empty_categories():
    """
    Endpoint to retrieve categories with no answers from the current user.

    :return: Response
        A JSON object containing the IDs and names of categories with no answers from the current user.
    """
    user_id = session.get('user_id') 
    
    category_info = {}

    categories = Category.query.all()

    for category in categories:
        category_data = {}
        questions = Question.query.filter_by(category_id=category.category_id).all()
        for question in questions:
            answer = Answer.query.filter_by(user_id=user_id, question_id=question.question_id).first()
            category_data[question.question_text] = answer.answer_text if answer else None
        if not any(category_data.values()):
            category_info[category.category_id] = category.category_name
            # print(category.category_id, category.category_name, "is empty")

    return jsonify(category_info)

@settings_bp.route('/get_current_categories', methods=['GET'])
def get_current_categories():
    """
    Endpoint to retrieve categories with at least one answer from the current user.

    :return: Response
        A JSON object containing the IDs and names of categories with at least one answer from the current user.
    """
    user_id = session.get('user_id') 
    
    category_info = {}

    categories = Category.query.all()

    for category in categories:
        if category.category_id == 1: # should not let them delete required questions
            continue
        category_data = {}
        questions = Question.query.filter_by(category_id=category.category_id).all()
        for question in questions:
            answer = Answer.query.filter_by(user_id=user_id, question_id=question.question_id).first()
            category_data[question.question_text] = answer.answer_text if answer else None
        if any(category_data.values()):
            category_info[category.category_id] = category.category_name
            
    return jsonify(category_info)

@settings_bp.route('/delete_categories', methods=['POST'])
def delete_categories():
    """
    Endpoint to delete all answers associated with selected categories for the current user.

    :param list categories: A list of category IDs to delete answers from.

    :return: Response
        A JSON object confirming the deletion of answers.
    """
    data = request.json
    user_id = session.get('user_id') 
    categories = data.get('categories')

    for category in categories:
        questions = Question.query.filter_by(category_id=category).all()
        for question in questions:
            Answer.query.filter_by(user_id=user_id, question_id=question.question_id).delete()
            db.session.commit()

    db.session.commit()

    return jsonify({'message': 'Answers deleted successfully'})

@settings_bp.route('/change_active_status', methods=['POST'])
def change_active_status():
    """
    Endpoint to toggle the active status of the current user (deactivate/reactivate a user).

    :return: Response
        A JSON object confirming the successful status change.
    """
    user_id = session.get('user_id')

    user = User.query.filter_by(user_id=user_id).first()
    user.deactivated = not user.deactivated
    
    db.session.commit()

    return jsonify({'message': 'Status changed successfully'})

@settings_bp.route('/check_active_status', methods=['GET'])
def check_active_status():
    """
    Endpoint to check the active status of the current user (whether the user is deactivated or not).

    :return: Response
        A JSON object indicating whether the user is active or not.
    """
    user_id = session.get('user_id')

    user = User.query.filter_by(user_id=user_id).first()

    return jsonify(not user.deactivated)

@settings_bp.route('/delete_account', methods=['POST'])
def delete_account():
    """
    Endpoint to delete the current user's account.

    :return: Response
        A JSON object indicating the success of the deletion process.
    """
    user_id = session.get('user_id') 

    if user_id is None:
        return jsonify({'error': 'User not authenticated'}), 401
    
    # delete answers
    answers = Answer.query.filter_by(user_id=user_id).all()
    for answer in answers:
        db.session.delete(answer)

    # delete pictures
    pictures = User_Photo.query.filter_by(user_id=user_id).all()
    for picture in pictures:
        db.session.delete(picture)

    # delete likes
    likes1 = Like.query.filter_by(user_id=user_id)
    for like in likes1:
        db.session.delete(like)
    likes2 = Like.query.filter_by(liked_user_id=user_id)
    for like in likes2:
        db.session.delete(like)

    # delete user
    user = User.query.filter_by(user_id=user_id).first()
    db.session.delete(user)    

    db.session.commit()

    return jsonify({'message': 'User deleted successfully'})

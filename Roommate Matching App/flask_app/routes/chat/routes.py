"""
Endpoints used for the chat window.
"""

from flask import Blueprint, jsonify, session
from database import db, User, Like
from sqlalchemy.orm import aliased

chat_bp = Blueprint('chat', __name__)

@chat_bp.route('/getcurrentuser', methods=['GET'])
def get_user():
    """
    Endpoint to retrieve information about the current authenticated user.

    :return: Response
        a JSON response containing user data for the current user.
        - If the user is not authenticated, returns a JSON response with an error message and a status code 401.
        - If the user is not found in the database, returns a JSON response with an error message and a status code 404.
    """
    current_user_id = session.get('user_id')

    if current_user_id is None:
        return jsonify({'error': 'User not authenticated'}), 401

    # Query to fetch email and password of the current user
    user = User.query.with_entities(User.email, User.password).filter(User.user_id == current_user_id).first()
    
    # Check if the user exists
    if user is None:
        return jsonify({'error': 'User not found'}), 404

    # Prepare user data to send to the frontend
    user_data = {
        'email': user.email,
        'password': user.password
    }

    return jsonify({'user': user_data}), 200
"""
Endpoints used for login.
"""
from test_data import major_options
from flask import Blueprint, request, jsonify, session, redirect, url_for
from database import db, User, Answer
from authlib.integrations.flask_client import OAuth
from run import oauth 
from werkzeug.security import generate_password_hash, check_password_hash
import requests
import os
import jwt

login_bp = Blueprint('auth', __name__)


def verify_google_token(access_token):
    """
    Verify a Google token using Authlib.

    :param str access_token: The access token to verify.

    :return: dict or None
        A dictionary containing user information if verification is successful, 
        None otherwise.
    """
    # Verify the Google token using Authlib
    google = oauth.create_client('google')  
    try:
        token = {'access_token': access_token}
        resp = google.get('userinfo', token=token)
        user_info = resp.json()
        return user_info
    except Exception as e:
        print(f"Token verification failed: {e}")
        return None

@login_bp.route('/gauthorize', methods=['POST','GET'])
def gauthorize():
    """
    Authorize a Google login request.

    :return: Response
        A JSON response indicating the success or failure of the login attempt.
    """
    user_info = request.json

    if user_info:
        email = user_info.get('email', '')
        user = User.query.filter_by(email=email).first()
        if user:
            session['user_id'] = user.user_id
            return jsonify({'success': True, 'message': 'User logged in successfully'}), 200
        
        verified = email.endswith('tamu.edu')
        new_user = User(
            email=email,
            first_name=user_info.get('given_name', None),    
            profile_photo=user_info.get('picture', None), 
            birthday=None,  
            gender=None,    
            major=None,     
            bio=None,       
            verified=verified,  
            # tamuEmail=None  
        )

        db.session.add(new_user)
        db.session.commit()

        session['user_id'] = new_user.user_id
        return jsonify({'success': True, 'message': 'User signed up and logged in successfully'}), 200
    else:
        return jsonify({'success': False, 'message': 'Invalid Google token'}), 401




@login_bp.route('/login', methods=['POST'])
def login():
    """
    Log in a user with email and password.

    :return: Response
        A JSON response indicating the success or failure of the login attempt.
    """
    data = request.json
    email = data.get('email')
    password = data.get('password')

    user = User.query.filter_by(email=email).first()

    if user and password and (user.password == password or check_password_hash(user.password, password)):
        session["user_id"] = user.user_id
        return jsonify({'message': 'Login successful'}), 200
    else:
        return jsonify({'message': 'Invalid email or password, if logging in with Google click the Google login button below.'}), 401
    


@login_bp.route('/signup', methods=['POST'])
def signup():
    """
    Register a new user.

    :return: Response
        A JSON response indicating the success or failure of the sign-up attempt.
    """
    data = request.json
    email = data.get('email')
    password = data.get('password')

    if email == None or password == None:
        return jsonify({'message': 'No user name and/or password provided'}), 404

    existing_user = User.query.filter_by(email=email).first()
    if existing_user:
        return jsonify({'message': 'User already exists'}), 400

    new_user = User(email=email, password=generate_password_hash(password))
    db.session.add(new_user)
    db.session.commit()

    session['user_id'] = new_user.user_id
    
    return jsonify({'message': 'Sign up successful'}), 201


@login_bp.route("/logout", methods=["POST"])
def logout_user():
    """
    Log out the current user.

    :return: Response
        A JSON response indicating the success or failure of the logout attempt.
    """
    if 'user_id' in session:
        session.pop('user_id', None)
        return jsonify({'message': 'Logged out successfully'}), 200
    else:
        return jsonify({'error': 'Not logged in'}), 401

@login_bp.route("/majors", methods=["GET"])
def get_majors():
    """
    Gets a list of available majors.

    :return: Response
        A JSON response containing the list of majors.
    """
    json_data = jsonify(major_options)
    return json_data

@login_bp.route('/profile_status', methods=['GET'])
def check_profile_status():
    """
    Check the profile completion status of the current user.

    :return: Response
        A JSON response containing the profile completion status.
    """
    user_id = session.get('user_id')
    if user_id is None:
        return jsonify({'error': 'No user id provided'}), 400

    user = User.query.get(user_id)
    incomplete_profile_status = user.first_name == None or user.birthday == None or user.major == None or user.gender == None or user.bio == None or user.profile_photo == None

    return jsonify({'profile_status': incomplete_profile_status}), 200
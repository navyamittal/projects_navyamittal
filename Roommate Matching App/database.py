"""
Contains initializations for SQLAlchemy database tables.
"""
from flask_sqlalchemy import SQLAlchemy
import requests
import os

db = SQLAlchemy()

class User(db.Model):
    """
    Represents a user in the application.

    Attributes:
        user_id (int): The unique identifier for the user.
        email (str): The email address of the user.
        password (str): The password of the user.
        first_name (str): The first name of the user.
        profile_photo (str): The file name of the user's profile photo.
        birthday (datetime.date): The birthday of the user.
        gender (str): The gender of the user.
        major (str): The major of the user.
        deactivated (bool): Indicates whether the user's account is deactivated.
        bio (str): The bio of the user.
        verified (bool): Indicates whether the user's tamu email is verified.
    """
    __tablename__ = 'users'

    user_id = db.Column(db.Integer, primary_key=True)
    email = db.Column(db.String(120), unique=True, nullable=False)
    password = db.Column(db.String(120), nullable=True)
    first_name = db.Column(db.String(120), nullable=True)
    profile_photo = db.Column(db.String(500), nullable=True)
    birthday = db.Column(db.Date, nullable=True)
    gender = db.Column(db.String(120), nullable=True)
    major = db.Column(db.String(120), nullable=True)
    deactivated = db.Column(db.Boolean, default=False, nullable=False)
    bio = db.Column(db.String(500), nullable=True)
    verified = db.Column(db.Boolean, nullable=False)
    # tamuEmail = db.Column(db.String(120), unique=True, nullable=True)

    def __init__(self, email, first_name = None, birthday = None, gender = None, major = None, bio = None, password = None,  profile_photo = None, verified=False, deactivated=False):
        self.email = email
        self.password = password
        self.first_name = first_name
        self.profile_photo = profile_photo
        self.birthday = birthday
        self.gender = gender
        self.major = major
        self.bio = bio
        self.verified = verified
        # self.tamuEmail = tamuEmail
        self.deactivated = deactivated

    def to_dict(self):
        user_dict = {
            "email": self.email,
            "first_name": self.first_name,
            "profile_photo": self.profile_photo if self.profile_photo else None,
            "gender": self.gender,
            "major": self.major,
            "birthday": self.birthday.strftime('%Y-%m-%d') if self.birthday else None,
            "verified": self.verified,
            # "tamuEmail": self.tamuEmail
        }

        return user_dict



class Like(db.Model):
    """
    Represents a like interaction between users.

    Attributes:
        user_id (int): The ID of the user who performed the like action.
        liked_user_id (int): The ID of the user who was liked.
        liked (bool): Indicates whether the like action was positive or negative.
    """
    __tablename__ = 'likes'
    
    user_id = db.Column(db.Integer, db.ForeignKey('users.user_id'), primary_key=True)
    liked_user_id = db.Column(db.Integer, db.ForeignKey('users.user_id'), primary_key=True)
    liked = db.Column(db.Boolean, nullable=False)
    
    def __init__(self, user_id, liked_user_id, liked):
        self.user_id = user_id
        self.liked_user_id = liked_user_id
        self.liked = liked



class User_Photo(db.Model):
    """
    Represents a user's photo.

    Attributes:
        photo_id (int): The unique identifier for the photo.
        user_id (int): The ID of the user who owns the photo.
        photo_url (str): The URL of the photo.
    """
    __tablename__='user_photos'
    
    photo_id = db.Column(db.Integer, primary_key=True)
    user_id = db.Column(db.Integer, db.ForeignKey('users.user_id'))
    photo_url = db.Column(db.String(244), nullable=False)
    
    def __init__(self, user_id, photo_url):
        self.user_id = user_id
        self.photo_url = photo_url
        
    
class Category(db.Model):
    """
    Represents a category for questions.

    Attributes:
        category_id (int): The unique identifier for the category.
        category_name (str): The name of the category.
    """
    __tablename__='categories'
    
    category_id = db.Column(db.Integer, primary_key=True)
    category_name = db.Column(db.String(120), nullable=False)
    
    def __init__(self, category_name):
        self.category_name = category_name
    
    
class Question(db.Model):
    """
    Represents a question.

    Attributes:
        question_id (int): The unique identifier for the question.
        category_id (int): The ID of the category to which the question belongs.
        question_text (str): The text of the question.
        question_type (str): The type of the question.
    """
    __tablename__='questions'
    
    question_id = db.Column(db.Integer, primary_key=True)
    category_id = db.Column(db.Integer, db.ForeignKey('categories.category_id'))
    question_text = db.Column(db.String(244), nullable=False)
    question_type = db.Column(db.String(20), nullable=False)  # Added this line

    def __init__(self, category_id, question_text, question_type):  # Updated the constructor
        self.category_id = category_id
        self.question_text = question_text
        self.question_type = question_type  # Added this line


    def serialize(self):
        return {
            'question_id': self.question_id,
            'category_id': self.category_id,
            'question_text': self.question_text,
            'question_type': self.question_type,  # Added this line
        }


class Answer(db.Model):
    """
    Represents an answer to a question.

    Attributes:
        user_id (int): The ID of the user who provided the answer.
        question_id (int): The ID of the question to which the answer belongs.
        answer_text (str): The text of the answer.
    """
    __tablename__='answers'
    user_id = db.Column(db.Integer, db.ForeignKey('users.user_id'), primary_key=True)
    question_id = db.Column(db.Integer, db.ForeignKey('questions.question_id'), primary_key=True)
    answer_text = db.Column(db.String(500), primary_key=True)
    
    def __init__(self, user_id, question_id, answer_text):
        self.user_id = user_id
        self.question_id = question_id
        self.answer_text = answer_text


class Answer_Choice(db.Model):
    """
    Represents a choice for a multiple-choice question.

    Attributes:
        question_id (int): The ID of the question to which the choice belongs.
        choice_text (str): The text of the choice.
    """
    __tablename__='answer_choices'

    question_id = db.Column(db.Integer, db.ForeignKey('questions.question_id'), primary_key=True)
    choice_text = db.Column(db.String(120), primary_key=True)

    def __init__(self, question_id, choice_text):
        self.question_id = question_id
        self.choice_text = choice_text
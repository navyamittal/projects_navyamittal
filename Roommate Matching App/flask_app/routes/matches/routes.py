"""
Endpoints used for the matches page
"""
from flask import Blueprint, jsonify, session
from database import db, User, Like
from sqlalchemy.orm import aliased
import os
from run import s3, s3_bucket, aws_region

matches_bp = Blueprint('matches', __name__)

@matches_bp.route('/getmatches', methods=['GET'])
def getmatches():
    """
    Get matches for the current user.

    :return: Response
        A JSON response containing information about the matches.
    """
    current_user_id = session.get('user_id')

    if current_user_id is None:
        return jsonify({'error': 'User not authenticated'}), 401

    # Create an alias for the Like model since we join on the same table
    LikeAlias = aliased(Like)

    # Query to find all the current user's likes that also like them back (a match for the current user)
    matches = Like.query\
        .join(LikeAlias, Like.user_id == LikeAlias.liked_user_id)\
        .filter(Like.liked_user_id == LikeAlias.user_id)\
        .filter(Like.liked == True, LikeAlias.liked == True)\
        .filter(Like.user_id == current_user_id)\
        .all()

    # Prepare match data to send to the frontend
    match_data = []
    for like in matches:
        match = User.query.filter(User.user_id == like.liked_user_id).first()

        #generate presigned link if in prod
        profile_photo = match.profile_photo
        if os.getenv('FLASK_ENV') == 'production':
            if profile_photo.startswith('aggies_house_'):
                s3_link = s3.generate_presigned_url(
                    'get_object',
                    Params={'Bucket': s3_bucket, 'Key': profile_photo},
                    ExpiresIn=300  # URL expires in 1 hour
                )
                profile_photo = s3_link

        match_info = {
            'user_id': match.user_id,
            'first_name': match.first_name,
            'major': match.major,
            'profile_photo': profile_photo,  # Assuming profile_photo is a URL
            # Add more fields as needed
            'email' : match.email
        }
        match_data.append(match_info)

    return jsonify({'matches': match_data}), 200

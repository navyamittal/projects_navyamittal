"""
Endpoints used for image display and submission.
"""
from flask import Blueprint, request, jsonify, session, send_file
from database import db, User, User_Photo
from werkzeug.utils import secure_filename
from werkzeug.datastructures import FileStorage
import os #remove later
import uuid
from run import s3, s3_bucket, aws_region
import urllib.parse

image_bp = Blueprint('images', __name__)


@image_bp.route('/photos', methods=['GET'])
def get_profile_images():
    """
    Retrieve profile images associated with the current user from AWS bucket.

    :return: Response
        A JSON response containing the URLs of the user's profile photos.
    """
    user_id = session.get('user_id')
    user = User.query.filter_by(user_id=user_id).first()

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


    return jsonify({'user_photos': photo_urls}), 200

@image_bp.route('/photos/upload', methods=['POST'])
def save_profile_images():
    """
    Save profile images associated with the current user.

    :return: Response
        A JSON response indicating whether the files were successfully uploaded.
    """
    user_id = session.get('user_id')
    if not user_id:
        return jsonify({'message': 'User not logged in'}), 401


    user = User.query.filter_by(user_id=user_id).first()
    if not user:
        return jsonify({'message': 'User not found'}), 404

    images = request.files.getlist('images[]')
    urls = request.form.getlist('urls[]')
    urlIdxs = request.form.getlist('urlIndexes[]')
    imageIdxs = request.form.getlist('imageIndexes[]')

    #merge into single array to keep proper ordering
    total_photos = len(urlIdxs) + len(imageIdxs)

    if total_photos == 0:
        return jsonify({'message': 'No photos sent'}), 400

    photos_arr = [None] * total_photos
    for index, image in enumerate(images):
        image_idx = int(imageIdxs[index])
        photos_arr[image_idx] = image

    for index,url in enumerate(urls):
        url_idx = int(urlIdxs[index])
        #for aws links, we want to keep just the file name
        if url.startswith(f'https://s3.{aws_region}.amazonaws.com/{s3_bucket}/'):
            photos_arr[url_idx] = extract_filename_from_presigned_url(url)
        else:
            photos_arr[url_idx] = url

    #create a set of photos that the user kept
    keep_photos_set = set()
    for photo in photos_arr:
        if isinstance(photo,str):
           keep_photos_set.add(photo)
    
    if os.getenv('FLASK_ENV') == 'development':
        #find out the current photos the user has
        current_photos = User_Photo.query.filter_by(user_id=user.user_id).all()
        if user.profile_photo != None:
            current_photos.insert(0,user.profile_photo)

        #remove photos that aren't kept by user from the db and files
        for index, photo in enumerate(current_photos):
            if index == 0 and photo not in keep_photos_set:
                #for profile photos, remove current
                user.profile_photo = None
                if photo.startswith('test_images/aggies_house_'):
                    os.remove(photo)
            elif index == 0:
                pass
            elif photo.photo_url not in keep_photos_set:
                #for non-profile photos, delete the current photo from directory a
                if photo.photo_url.startswith('test_images/aggies_house_'):
                    os.remove(photo.photo_url)
                db.session.delete(photo)
            elif index > 0:
                db.session.delete(photo)

        db.session.commit()
    
        # add new images back to db
        for index, image in enumerate(photos_arr):
            upload_photo_local(user, image, index)

        # #find out the current photos the user has
        # current_photos = User_Photo.query.filter_by(user_id=user.user_id).all()
        # current_photos.insert(0,user.profile_photo)

        # #remove old photos that aren't kept by user and remove all old user_photos
        # for index, photo in enumerate(current_photos):
        #     if index == 0 and photo not in keep_photos_set:
        #         #for profile photos, remove current
        #         user.user_photo = None
        #         if photo.startswith('aggies_house_'):
        #             s3.delete_object(Bucket=s3_bucket, Key=photo)
        #     elif index == 0:
        #         pass
        #     elif photo.photo_url not in keep_photos_set:
        #         #for non-profile photos, delete the current photo from directory a
        #         if photo.photo_url.startswith('aggies_house_'):
        #             s3.delete_object(Bucket=s3_bucket, Key=photo.photo_url)
        #         db.session.delete(photo)
        #     elif index > 0:
        #         db.session.delete(photo)

        # db.session.commit()
    
        # # add images back to db
        # for index, image in enumerate(photos_arr):
        #     upload_photo_aws(user, image, index)

    elif os.getenv('FLASK_ENV') == 'production':
        #find out the current photos the user has
        current_photos = User_Photo.query.filter_by(user_id=user.user_id).all()
        if user.profile_photo != None:
            current_photos.insert(0,user.profile_photo)

        #remove old photos that aren't kept by user and remove all old user_photos
        for index, photo in enumerate(current_photos):
            if index == 0 and photo not in keep_photos_set:
                #for profile photos, remove current
                user.user_photo = None
                if photo.startswith('aggies_house_'):
                    s3.delete_object(Bucket=s3_bucket, Key=photo)
            elif index == 0:
                pass
            elif photo.photo_url not in keep_photos_set:
                #for non-profile photos, delete the current photo from directory a
                if photo.photo_url.startswith('aggies_house_'):
                    s3.delete_object(Bucket=s3_bucket, Key=photo.photo_url)
                db.session.delete(photo)
            elif index > 0:
                db.session.delete(photo)

        db.session.commit()
    
        # add images back to db
        for index, image in enumerate(photos_arr):
            upload_photo_aws(user, image, index)
        
    saved_files = True

    if saved_files:
        db.session.commit()
        return jsonify({'message': 'Files successfully uploaded'}), 200
    else:
        return jsonify({'message': 'Failed to upload files'}), 500

def allowed_file(filename):
    """
    Check if a file has an allowed extension.

    :param str filename: The name of the file to check.

    :return: bool
        True if the file has an allowed extension, False otherwise.
    """
    ALLOWED_EXTENSIONS = {'png', 'jpg', 'jpeg'}
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

def upload_photo_local(user, image, index):
    """
    Upload a photo locally or set it as the user's profile photo.

    :param User user: The user for whom the photo is being uploaded.
    :param str or FileStorage image: The image to upload.
    :param int index: The index of the image in the array of photos.

    :return: None
    """
    if isinstance(image, str):
        if index == 0:
            user.profile_photo = image
        else:
            new_photo = User_Photo(user_id=user.user_id, photo_url=image)
            db.session.add(new_photo)      
    elif isinstance(image, FileStorage):
        if index != 0 and allowed_file(image.filename):
            original_filename = secure_filename(image.filename)
            original_extension = os.path.splitext(original_filename)[1]
            hash_value = uuid.uuid4().hex
            file_path = f'test_images/aggies_house_{hash_value}{original_extension}'
            image.save(file_path)
            new_photo = User_Photo(user_id=user.user_id, photo_url=file_path)
            db.session.add(new_photo)

        elif index == 0 and allowed_file(image.filename):
            original_filename = secure_filename(image.filename)
            original_extension = os.path.splitext(original_filename)[1]
            hash_value = uuid.uuid4().hex
            file_path = f'test_images/aggies_house_{hash_value}{original_extension}'
            image.save(file_path)
            user.profile_photo = file_path

def upload_photo_aws(user,image,index):
    """
    Upload a photo to AWS S3 or set it as the user's profile photo.

    :param User user: The user for whom the photo is being uploaded.
    :param str or FileStorage image: The image to upload.
    :param int index: The index of the image in the array of photos.

    :return: None
    """
    if isinstance(image, str):
        if index == 0:
            user.profile_photo = image
        else:
            new_photo = User_Photo(user_id=user.user_id, photo_url=image)
            db.session.add(new_photo)      
    elif isinstance(image, FileStorage):
        if index != 0 and allowed_file(image.filename):
            original_filename = secure_filename(image.filename)
            original_extension = os.path.splitext(original_filename)[1]
            hash_value = uuid.uuid4().hex
            file_path = f'aggies_house_{hash_value}{original_extension}'
            s3.upload_fileobj(
                image,
                s3_bucket,
                file_path
            )
            new_photo = User_Photo(user_id=user.user_id, photo_url=file_path)
            db.session.add(new_photo)

        elif index == 0 and allowed_file(image.filename):
            original_filename = secure_filename(image.filename)
            original_extension = os.path.splitext(original_filename)[1]
            hash_value = uuid.uuid4().hex
            file_path = f'aggies_house_{hash_value}{original_extension}'
            s3.upload_fileobj(
                image,
                s3_bucket,
                file_path
            )
            user.profile_photo = file_path

def extract_filename_from_presigned_url(presigned_url):
    """
    Extract the filename from a presigned URL.

    :param str presigned_url: The presigned URL from which to extract the filename.

    :return: str
        The extracted filename.
    """
    parsed_url = urllib.parse.urlparse(presigned_url)

    path = parsed_url.path
    
    object_key = path.split('/')[-1]
    
    filename = urllib.parse.unquote(object_key)
    
    return filename

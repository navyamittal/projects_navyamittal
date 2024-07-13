from flask import Flask, send_from_directory, jsonify, request, session
from flask_cors import CORS, cross_origin
from dotenv import load_dotenv
from sqlalchemy_utils import database_exists, create_database
from database import *
import os
from sqlalchemy import text
# from flask_mail import Mail, Message
from authlib.integrations.flask_client import OAuth
import boto3
from botocore.client import Config
from flask_socketio import SocketIO, emit, join_room, leave_room

app = Flask(__name__, static_folder='frontend/build', static_url_path='')
CORS(app, origins=['http://localhost:5000', 'http://127.0.0.1:5000', 'https://aggies.house', 'https://aggieshouse-8782c141a8c4.herokuapp.com', 'https://www.aggies.house'], resources={r"/*":{"origins":"*"}})
# mail = Mail(app)

oauth = OAuth(app)

oauth.register(
    name='google',
    client_id=  os.getenv('OAUTH_CLIENT_ID'),
    client_secret=  os.getenv('OAUTH_CLIENT_SECRET'),
    access_token_url='https://accounts.google.com/o/oauth2/token',
    access_token_params=None,
    authorize_url='https://accounts.google.com/o/oauth2/auth',
    authorize_params=None,
    api_base_url='https://www.googleapis.com/oauth2/v1/',
    userinfo_endpoint='https://openidconnect.googleapis.com/v1/userinfo',  # This is only needed if using openId to fetch user info
    client_kwargs={'scope': 'email profile'},
    server_metadata_url='https://accounts.google.com/.well-known/openid-configuration'
)

socketio = SocketIO(app, cors_allowed_origin = "*")

#db setup
load_dotenv() #only needed for local?
if os.getenv('FLASK_ENV') == 'production':
    database_uri = os.getenv('DATABASE_URL')
    if database_uri.startswith("postgres://"):
        database_uri = database_uri.replace("postgres://", "postgresql://", 1)
else:
    user,passwd,host,port,pgdb = os.getenv('pguser'), os.getenv('pgpass'), os.getenv('pghost'), os.getenv('pgport'), os.getenv('pgdb')
    database_uri = f"postgresql://{user}:{passwd}@{host}:{port}/{pgdb}"
    
    if not database_exists(database_uri):
        create_database(database_uri)

app.config['SQLALCHEMY_DATABASE_URI'] = database_uri
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

aws_access_key = os.getenv('AWS_ACCESS_KEY')
aws_secret_key = os.getenv('AWS_SECRET_KEY')
s3_bucket = os.getenv('S3_BUCKET')
aws_region = os.getenv('AWS_REGION')

s3 = boto3.client('s3', aws_access_key_id=aws_access_key, aws_secret_access_key = aws_secret_key, region_name = aws_region, endpoint_url = f'https://s3.{aws_region}.amazonaws.com')

db.init_app(app)
app.secret_key = os.getenv('SECRET_KEY', 'default_secret_key')
with app.app_context():
    db.drop_all() #if you make changes to the db, you'll have to drop all tables if they have already been created before another change or we can use db migrations using alembic
    db.create_all()

    # for test users
    from testusers import load_test_data
    if User.query.count() == 0:
       load_test_data(db)

#routes + bps
@app.route('/')
@app.route('/login')
@app.route('/login2')
@app.route('/signup')
@app.route('/feed')
@app.route('/chat')
@app.route('/settings')
@app.route('/matches')
@app.route('/questionnaire')
@app.route('/complete_profile')
@app.route('/questionnaire2')
@app.route('/imageupload')
@cross_origin()

def index():
    return send_from_directory(app.static_folder, 'index.html')


# @socketio.on("connect")
# def connected():
#     """event listener when client connects to the server"""
#     print(request.sid)
#     print("client has connected")
#     emit("userConnected",{"data":f"id: {request.sid} is connected"})


# @socketio.on("connect")
# def connected(matches):
#     """Event listener when client connects to the server"""
#     print(request.sid)
#     print("Client has connected")
#     emit("userConnected", {"data": f"id: {request.sid} is connected"})

#     # Fetch matches from the server
#     #matches_response = axios.get('http://your-api-url/getmatches')
#     #matches = matches.json().get('matches', [])

#     # Emit matches to the connected client
#     emit("matches", matches)

#     # Establish socket connections between matched users
#     for match in matches:
#         # Check if the current user matches with this match
#         #if match.get('user_id') == request.sid:  # Example logic to check match
#             # Get the socket ID of the matched user
#             matched_socket_id = match.user_id  # Replace with actual logic to get socket ID

#             # Establish socket connection with the matched user
#             emit("matchedConnection", {"userId": request.sid}, room=matched_socket_id)

# @socketio.on('joinRoom')
# def join_room_event(roomId):
#     join_room(roomId)
#     print(f"User joined room {roomId}")

# @socketio.on('leaveRoom')
# def leave_room_event(roomId):
#     leave_room(roomId)
#     print(f"User left room {roomId}")

# @socketio.on('joinRoom')
# def join_room_event(data):
#     """Event listener to join a specific room"""
#     room_name = str(data)
#     if room_name:
#         join_room(room_name)
#         print("join room from the front end: ",str(data))
#         emit("roomJoined", {"data": f"You have joined room: {room_name}"})
#     else:
#         emit("joinRoomError", {"error": "Invalid room name"})

# @socketio.on("joinRoom")
# def handle_join_room(data):
#     user_id = data.get("userId")
#     room_id = f"room_{user_id}"
#     print(f"User {user_id} joined room {room_id}")
#     join_room(room_id)

# @socketio.on("data")
# def handle_data(data):
#     print("data: ", str(data))
#     user_id = data.get("userId")
#     room_id = f"room_{user_id}"
#     print("Received data in room: ", str(room_id))
#     emit("data", data, room=room_id, include_self=False)

# @socketio.on('data')
# def handle_message(data):
#     """event listener when client types a message"""
#     print("data from the front end: ",str(data))
#     emit("data",{'data':data,'id':request.sid},broadcast=True)
    

# @socketio.on("disconnect")
# def disconnected():
#     """event listener when client disconnects to the server"""
#     print("user disconnected")
#     emit("disconnect",f"user {request.sid} disconnected",broadcast=True)


from flask_app.routes.login.routes import *
app.register_blueprint(login_bp)

from flask_app.routes.feed.routes import *
app.register_blueprint(feed_bp)

from flask_app.routes.chat.routes import *
app.register_blueprint(chat_bp)

from flask_app.routes.questionnaire.routes import *
app.register_blueprint(question_bp)

from flask_app.routes.settings.routes import *
app.register_blueprint(settings_bp)

from flask_app.routes.matches.routes import *
app.register_blueprint(matches_bp)

# app.config['MAIL_SERVER'] = 'smtp.fastmail.com'
# app.config['MAIL_PORT'] = 465
# app.config['MAIL_USE_SSL'] = True
# app.config['MAIL_USE_TLS'] = False
# app.config['MAIL_USERNAME'] = 'aggieshouse@fastmail.com'
# app.config['MAIL_PASSWORD'] = 'wr2grs44gtleuygf'
# app.config['MAIL_DEBUG'] = True

from flask_app.routes.images.routes import *
app.register_blueprint(image_bp)

if os.getenv('FLASK_ENV') == 'development':
    @app.route('/test_images/<path:filename>')
    def serve_test_image(filename):
        """
        Serve files from the test_images directory.
        """
        return send_from_directory('test_images', filename)
    
# Define your Chat Engine API endpoint and private key
chat_engine_url = 'https://api.chatengine.io/users/'
chat_engine_private_key = os.getenv('CHAT_ENGINE_PRIVATE_KEY')  # Replace with your Chat Engine private key

def migrate_existing_users_to_chat_engine():
    """
    Migrates existing users to Chat Engine.

    Returns:
        tuple: A tuple containing lists of new users migrated to Chat Engine and failed users.
    """
    with app.app_context():  # Ensure execution within Flask app context
        users = User.query.all()  # Get all existing users from your database

        new_users = []
        failed_users = []

        for user in users:
            chat_engine_data = {
                "username": user.email,  # Using email as username for simplicity, you can adjust this as needed
                "secret": user.password,  # Using password as secret for simplicity, ensure it's secure in production
                "email": user.email,
                "first_name": user.first_name,
                # Add other fields as needed
            }

            headers = {'PRIVATE-KEY': chat_engine_private_key}

            try:
                response = requests.post(chat_engine_url, headers=headers, json=chat_engine_data)
                response.raise_for_status()  # Raise exception if request fails

                if response.status_code == 201:
                    new_users.append(response.json())
                else:
                    failed_users.append(response.json())
            except requests.exceptions.RequestException as e:
                failed_users.append({'error': str(e), 'user_id': user.user_id})  # Add user ID for reference in case of failure

        return new_users, failed_users

# Example usage
if __name__ == '__main__':
    new_users, failed_users = migrate_existing_users_to_chat_engine()
    print('New users migrated to Chat Engine:', new_users)
    print('Failed users:', failed_users)



"""
File used to generate test users.
"""
from datetime import datetime, timedelta
from database import *
from random import choice, randint, sample
from test_data import first_names, major_options, photos, categories_and_questions

def load_test_data(db):
    """
    Load test data into the database.

    Parameters:
    - db: SQLAlchemy database session

    Returns:
    - None
    """
    # Start date for birthdays
    start_date = datetime(1995, 1, 1)
    # End date for birthdays
    end_date = datetime(2005, 12, 31)

    for i in range(1, 10):
        first_name = choice(first_names)
        random_days = randint(0, (end_date - start_date).days)
        birthday = start_date + timedelta(days=random_days)
        gender = choice(["Male", "Female", "Non-binary"])
        major = choice(major_options)
        email = f"example{i}@example.com"
        password = "test"
        profile_photo = choice(photos)
        bio = f"Hi, I am {first_name}"

        verified = choice([True, False])
        # if verified:
        #     tamuEmail = f"example{i}@tamu.edu"
        # else:
        #     tamuEmail = None

        # Create user object
        user = User(
            email=email,
            password=password,
            first_name=first_name,
            birthday=birthday,
            gender=gender,
            major=major,
            profile_photo=profile_photo,
            bio=bio,
            verified = verified
            # tamuEmail=tamuEmail
        )
        
        db.session.add(user)

        chat_engine_url = 'https://api.chatengine.io/users/'
        chat_engine_private_key = os.getenv('CHAT_ENGINE_PRIVATE_KEY')  # Replace with your Chat Engine private key
        headers = {'PRIVATE-KEY': chat_engine_private_key}
        chat_engine_data = {
            "username": user.email,  # Using email as username for simplicity, you can adjust this as needed
            "secret": user.password,  # Using password as secret for simplicity, ensure it's secure in production
            "email": user.email,
            "first_name": user.first_name,
            # Add other fields as needed
        }

        try:
            response = requests.post(chat_engine_url, headers=headers, json=chat_engine_data)
            response.raise_for_status()  # Raise exception if request fails
        except requests.exceptions.RequestException as e:
            pass

    # Commit all users to the database
    db.session.commit()

    users = User.query.all()

    for user in users:
        photo1 = User_Photo(user_id=user.user_id, photo_url= choice(photos))
        photo2 = User_Photo(user_id=user.user_id, photo_url= choice(photos))
        db.session.add(photo1)
        db.session.add(photo2)
        db.session.commit()

    for category_name, questions in categories_and_questions.items():
        category = Category(category_name=category_name)
        db.session.add(category)
        db.session.commit()

        for question_data in questions:
            question_text = question_data.get("text")
            question_type = question_data.get("type")
            options = question_data.get("options")
            question = Question(category_id=category.category_id, question_text=question_text, question_type=question_type)
            db.session.add(question)
            db.session.commit()

            if options:
                for option in options:
                    answer_choice = Answer_Choice(question_id=question.question_id, choice_text=option)
                    db.session.add(answer_choice)
                db.session.commit()

            for user in users:
                if question_type == "multiple choice" and options:
                    answer_text = choice(options)
                    answer = Answer(question_id=question.question_id, user_id=user.user_id, answer_text=answer_text)
                    db.session.add(answer)
                #randomly select between 1 and n options
                elif question_type == "multi select" and options:
                    if len(options) > 5:
                        num_selected = randint(1,5)
                    else:
                        num_selected = randint(1, len(options))
                    selected_answers = sample(options, num_selected)
                    for selected_answer in selected_answers:
                        answer = Answer(question_id=question.question_id, user_id=user.user_id, answer_text=selected_answer)
                        db.session.add(answer)
                else:
                    answer_text = f"{user.first_name} sample answer"
                    answer = Answer(question_id=question.question_id, user_id=user.user_id, answer_text=answer_text)
                    db.session.add(answer)
                db.session.commit()

    #For testing matches
    test_user = User.query.first()
    users = User.query.filter(User.user_id != test_user.user_id).all()
    for i, user in enumerate(users, start=1):
        if i <= 4:
            like = Like(user_id=user.user_id, liked_user_id=test_user.user_id, liked=True)
            db.session.add(like)
            match_like = Like(user_id=test_user.user_id, liked_user_id=user.user_id, liked=True)
            db.session.add(match_like)
        else:
            break
    db.session.commit()
    
    

    #Test user profile for incomplete info
    incomplete_user = User(
        email="incomplete@example.com",
        password="test",
        first_name=None,
        birthday=None,
        gender=None,
        major=None,
        profile_photo=None,
        bio=None,
        verified = False
        # tamuEmail=tamuEmail
    )

    db.session.add(incomplete_user)
    db.session.commit()

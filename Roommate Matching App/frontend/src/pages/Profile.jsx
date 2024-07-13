import React, { useState, useEffect } from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faBirthdayCake, faGraduationCap, faVenusMars, faPen, faCirclePlus, faCircleXmark, faPowerOff, faMoon, faTrash, faPlus, faUserCheck, faImage} from '@fortawesome/free-solid-svg-icons';
import './feed.css';
import './Settings.css';
import { useNavigate } from 'react-router-dom';
import ImageUpload from '../components/ImageUpload';

import axios from 'axios';

/**
 * Frontend component for viewing profile under the Account page.
 * @component
 * @param {function} toggleEditView - Function to toggle Account page view between viewing and editing.
 * @param {function} toggleCatView - Function to toggle Account page view between adding and deleting categories or viewing profile.
 */
function Profile({toggleEditView, toggleCatView}) {
    const [userId, setUserId] = useState(null);
    const [currentImageIndex, setCurrentImageIndex] = useState(0);
    const [currentUser, setCurrentUser] = useState({});
    const [isActive, setIsActive] = useState(null);
    const [isVisible, setIsVisible] = useState(false);
    const [imagesUploaded, setImagesUploaded] = useState(true);
    const [formSubmitted, setFormSubmitted] = useState(false);

    const navigate = useNavigate();

    useEffect(() => {
        fetchUserId(); 
        fetchActiveStatus();
    }, []);

    useEffect(() => {
        if (userId !== null) {
            fetchProfileInfo(userId);
        }
    }, [userId]);

    /**
     * Fetches the user ID from the backend.
     */
    const fetchUserId = async () => {
        await axios.get(`/get_user_id`)
            .then(response => {
                setUserId(response.data.user_id);
            })
            .catch(error => {
                console.error('Error fetching user ID:', error);
            });
    };

    /**
     * Fetches profile information for a given user ID from the backend.
     * @param {string} userId - The ID of the user.
     */
    const fetchProfileInfo = async (userId) => {
        // Fetch profile information for the given user ID
        await axios.get(`/userinfo?user_id=${userId}`)
            .then(response => {
                // Set the current user with the fetched profile data
                setCurrentUser(response.data);
            })
            .catch(error => {
                console.error('Error fetching profile information:', error);
            });
    };

    /**
     * Fetches the active status of the user from the backend.
     */
    const fetchActiveStatus = async () => {
        await axios.get('/settings/check_active_status')
            .then(response => {
                setIsActive(response.data);
            })
            .catch(error => {
                console.error('Error fetching active status:', error);
            });
    }

    /**
     * Handles the change in image to the next one in the carousel.
     */
    const handleNextImage = () => {
        setCurrentImageIndex((prevIndex) => (prevIndex + 1) % currentUser.photos.length);
    };

    /**
     * Handles the change in image to the previous one in the carousel.
     */
    const handlePrevImage = () => {
        setCurrentImageIndex((prevIndex) => (prevIndex - 1 + currentUser.photos.length) % currentUser.photos.length);
    };

    /**
     * Handles the change in account active status.
     */
    const handleStatusChange = async () => {
        try {
            await axios.post('/settings/change_active_status');
            setIsActive(!isActive);
            console.log('Account status changed successfully');
        } catch (error) {
            console.error('Error changing account status:', error);
        }
    }

    /**
     * Handles the deletion of the user's account.
     */
    const handleDelete = async () => {
        try {
            await axios.post('/settings/delete_account');
            console.log('User deleted successfully');
        } catch (error) {
            console.error('Error deleting user:', error);
        }
    };

    const handleShowImageUpload = () => {
        setIsVisible(true);
        document.body.classList.add('popup-open');
    };

    return (
        <div className="feedcontainer" style={{marginTop:'0'}}>
            <>
            <h2 className='settings-header'>View/Edit Profile</h2>
                <div className="profilecontainer" style={{ display: 'flex', justifyContent: 'center', alignItems: 'center'}}>
                    <div className="profile-info">
                        <h2 className='mt-3'>{currentUser.first_name} {currentUser.last_name}</h2>
                        <div className="text-center">
                            <div className="carousel slide mb-2 rounded-5" data-ride="carousel">
                                <div className="carousel-inner rounded-5">
                                {currentUser && Array.isArray(currentUser.photos) && currentUser.photos.map((imageUrl, index) => (
                                    <div className={`carousel-item ${index === currentImageIndex ? 'active' : ''}`} key={index}>
                                        <img src={imageUrl} className="d-block w-100" style={{ height: '500px' }} alt={`Image ${index}`} />
                                    </div>
                                ))}

                                </div>
                                <a className="carousel-control-prev" role="button" data-slide="prev" onClick={handlePrevImage}>
                                    <span className="carousel-control-prev-icon" aria-hidden="true"></span>
                                    <span className="sr-only">Previous</span>
                                </a>
                                <a className="carousel-control-next" role="button" data-slide="next" onClick={handleNextImage}>
                                    <span className="carousel-control-next-icon" aria-hidden="true"></span>
                                    <span className="sr-only">Next</span>
                                </a>
                            </div>
                            <button className="upload-image-button" onClick={handleShowImageUpload}>
                                <FontAwesomeIcon icon={faImage} className="icon" />
                                Edit Images
                            </button>
                            {isVisible && <ImageUpload isVisible={isVisible} setIsVisible={setIsVisible} imagesUploaded={imagesUploaded} setImagesUploaded={setImagesUploaded} setFormSubmitted={setFormSubmitted} formSubmitted={formSubmitted} />}
                        </div>
                        <div className="row justify-content-around mb-1">
                            {currentUser.verified === true && (
                                <div className="col-md-4 text-center">
                                    <FontAwesomeIcon icon={faUserCheck} style={{ fontSize: '1.5rem' }} />
                                    <p className="mt-1">Verified</p>
                                </div>
                                )
                            }
                            <div className="col-md-4 text-center">
                                <FontAwesomeIcon icon={faBirthdayCake} style={{ fontSize: '1.5rem' }} />
                                <p className="mt-1">{currentUser.age}</p>
                            </div>
                            <div className="col-md-4 text-center">
                                <FontAwesomeIcon icon={faGraduationCap} style={{ fontSize: '1.5rem' }} />
                                <p className="mt-1">{currentUser.major}</p>
                            </div>
                            <div className="col-md-4 text-center">
                                <FontAwesomeIcon icon={faVenusMars} style={{ fontSize: '1.5rem' }} />
                                <p className="mt-1">{currentUser.gender}</p>
                            </div>
                        </div>
                        <div className="card shadow rounded-5 mb-4">
                            <div className="row no-gutters">
                                <div className="col-md-12">
                                    <div className="card-body">
                                        <h5 className="card-title">About Me: </h5>
                                        <p className="card-text">{currentUser.bio}</p>
                                    </div>
                                </div>
                            </div>
                        </div>
                        {currentUser && currentUser.categories && Object.entries(currentUser.categories).map(([category, questions], categoryIndex) => (
                            <div key={categoryIndex}>
                                <h4 className="text-center mb-2">{category}</h4>
                                <div className="card shadow rounded-5 mb-4">
                                    <div className="row no-gutters">
                                        <div className="col-md-12 edit-card">
                                            <div className="card-body">
                                                {Object.entries(questions).map(([question, answer], index) => (
                                                    <div key={index}>
                                                        <p className="card-subtitle text-muted">{question}</p>
                                                        {/* Check if the answer is an array */}
                                                        {Array.isArray(answer) ? (
                                                            // If it is, map over the array and display each item on its own line
                                                            <div>
                                                                {answer.map((item, itemIndex) => (
                                                                    <h6 key={itemIndex} className={`card-text ${itemIndex === answer.length - 1 ? 'mb-3' : 'mb-1'}`}>
                                                                        {item}
                                                                    </h6>
                                                                ))}
                                                            </div>
                                                        ) : (
                                                            // If it's not an array, display the answer as before
                                                            <h6 className="card-text mb-3">{answer}</h6>
                                                        )}
                                                    </div>
                                                ))}
                                            </div>
                                            <div className="edit-btn-div">
                                                <button className='btn edit-btn' onClick={() => toggleEditView(category)}>
                                                    <FontAwesomeIcon icon={faPen} style={{ color: 'white' }} />
                                                </button>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        ))}
                        <div className='category-btn-div'>
                            <button className='btn categories-btn' onClick={() => toggleCatView(1)}>
                            <FontAwesomeIcon icon={faPlus} /> Add Categories
                            </button>
                            <button className='btn categories-btn' onClick={() => toggleCatView(2)}>
                            <FontAwesomeIcon icon={faTrash} />  Delete Categories
                            </button>
                        </div>
                        <hr className='settings-hr'></hr>
                        <h3 className='settings-header'>Account Settings</h3>
                        { isActive ? (
                            <h4 className="text-center mb-2">Deactivate Account</h4>
                            ) : (
                            <h4 className="text-center mb-2">Reactivate Account</h4>
                        )}
                        <div className="card shadow rounded-5 mb-4">
                            <div className="row no-gutters">
                                <div className="col-md-12 card-parent">
                                    <div className="card-body">
                                        { isActive ? (
                                            <div>
                                                <p className="card-text">
                                                    If you are satisfied with your current matches and do not want your profile to be shown in other users' feeds, you can temporarily deactivate your account.  
                                                    You will retain access to your current matches and chats, but you will lose access to your feed until you choose to reactivate your account.
                                                </p>
                                                <div className='category-btn-div'>
                                                    <button className='btn categories-btn' onClick={() => handleStatusChange()}>
                                                    <FontAwesomeIcon icon={faMoon} /> Deactivate Account
                                                    </button>
                                                </div>
                                            </div>
                                        ) : (
                                            <div>
                                                <p className="card-text">
                                                    Your account is currently deactivated. If you would like to form new matches, you can reactivate your account below: this will give you access to your feed
                                                    and your profile will be shown to other users again.
                                                </p>
                                                <div className='category-btn-div'>
                                                    <button className='btn categories-btn' onClick={() => handleStatusChange()}>
                                                    <FontAwesomeIcon icon={faPowerOff} /> Reactivate Account
                                                    </button>
                                                </div>
                                            </div>
                                        )}
                                    </div>
                                </div>
                            </div>
                        </div>
                        <h4 className="text-center mb-2">Delete Account</h4>
                        <div className="card shadow rounded-5 mb-4">
                            <div className="row no-gutters">
                                <div className="col-md-12 card-parent">
                                    <div className="card-body">
                                        <p className="card-text">
                                            This will <strong>permanently</strong> delete your account and all associated information, including your questionnaire responses, likes, and matches. 
                                            If you are sure you would like to proceed, click the button below. Your account will be deleted and you will be redirected to the home page.
                                        </p>
                                        <div className='category-btn-div'>
                                            <button className='btn categories-btn' onClick={() => {handleDelete(); navigate('/')}}>
                                            <FontAwesomeIcon icon={faTrash} /> Delete Account
                                            </button>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>                
            </>

            <style jsx>{`
                .type-writer-container {
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    height: 100%;
                    margin-top: 50px;
                    font-size: 800px;
                    fontWeight: 'bold'
                }
                .type-writer-text {
                    font-size: 100px; /* Adjust the font size as needed */
                    color: rgb(0,0,0); /* Adjust text color */
                    font-family: "Montserrat", sans-serif;
                }
            `}</style>
        </div>
    );
}

export {Profile};
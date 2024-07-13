import React, { useState, useEffect } from 'react';
import { useSwipeable } from 'react-swipeable';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faThumbsUp, faThumbsDown, faTimes, faArrowRight, faBirthdayCake, faGraduationCap, faVenusMars, faSpinner, faUserCheck  } from '@fortawesome/free-solid-svg-icons';
import './feed.css';
import axios from 'axios';
import MatchedPopup from '../components/MatchedPopup';
import Typewriter from 'react-typewriter-effect';
import '../components/MatchedPopup.css'

/**
 * Frontend component for main feed.
 * @component
 */
function Feed() {
    const [profiles, setProfiles] = useState([]);
    const [currentProfileIndex, setCurrentProfileIndex] = useState(0);
    const [currentImageIndex, setCurrentImageIndex] = useState(0);
    const [currentUser, setCurrentUser] = useState({});
    const [showPopup, setShowPopup] = useState(false);
    const [disableSwipe, setDisableSwipe] = useState(false);
    const [loading, setLoading] = useState(true);
    const [dotIndex, setDotIndex] = useState(0);
    const [isActive, setIsActive] = useState(true);

    useEffect(() => {
        fetchActiveStatus();
    }, []);

    useEffect(() => {
        // Fetch user IDs when the component mounts
        axios.get('/getfeed')
            .then(response => {
                const user_ids = response.data.user_ids;
                // Set the list of user IDs to state
                setProfiles(user_ids);
                setLoading(false); // Mark loading as false after fetching data
            })
            .catch(error => {
                console.error('Error fetching user feed:', error);
                setLoading(false); // Mark loading as false in case of error
            });
    }, []);

    useEffect(() => {
        // Check if profiles have been set and not empty
        if (profiles.length > 0) {
            // Fetch profile information for the first user ID
            fetchProfileInfo(profiles[0]);
        }
    }, [profiles]);

    /**
     * Moves to the next image.
     */
    const handleNextImage = () => {
        setCurrentImageIndex((prevIndex) => (prevIndex + 1) % currentUser.photos.length);
    };

    /**
     * Moves to the previous image.
     */
    const handlePrevImage = () => {
        setCurrentImageIndex((prevIndex) => (prevIndex - 1 + currentUser.photos.length) % currentUser.photos.length);
    };


    useEffect(() => {
        // Disable swipe when the popup is shown
        if (showPopup) {
            setDisableSwipe(true);
        } else {
            setDisableSwipe(false);
        }
    }, [showPopup]);

    /**
     * Handles liking a profile.
     */
    const handleLike = () => {
        const likedUserId = profiles[currentProfileIndex];
        //let navigate = useNavigate();
        axios.post('/like', { liked_user_id: likedUserId })
        .then(response => {
            if (response.data.match) {
                setShowPopup(true);
            }
            
            showNextProfile();
        })
        .catch(error => {
            console.error('Error liking profile:', error);
            showNextProfile();
        });
    };

     /**
     * Handles disliking a profile.
     */
    const handleDislike = () => {
        const likedUserId = profiles[currentProfileIndex];

        axios.post('/dislike', { liked_user_id: likedUserId })
        .then(response => {
            showNextProfile();
        })
        .catch(error => {
            console.error('Error disliking profile:', error);
            showNextProfile();
        });
    };

    /**
     * Fetches profile information for the given user ID.
     * @param {string} userId - The ID of the user.
     */
    const fetchProfileInfo = (userId) => {
        // Fetch profile information for the given user ID
        axios.get(`/userinfo?user_id=${userId}`)
            .then(response => {
                // Set the current user with the fetched profile data
                setCurrentUser(response.data);
            })
            .catch(error => {
                console.error('Error fetching profile information:', error);
            });
    };

    /**
     * Shows the next profile.
     */
    const showNextProfile = () => {
        // Increment the profile index to show the next profile
        const nextIndex = currentProfileIndex + 1;
        if (nextIndex < profiles.length) {
            setCurrentProfileIndex(nextIndex);
            // Fetch and set the profile info for the next user
            fetchProfileInfo(profiles[nextIndex]);
            setCurrentImageIndex(0);
        } else {
            setCurrentProfileIndex(nextIndex);
        }
    };

    /**
     * Handles swipe actions.
     */
    const swipeHandlers = useSwipeable({
        onSwipedLeft: handleDislike,
        onSwipedRight: handleLike,
        trackMouse: true
    });


    useEffect(() => {
        const timer = setInterval(() => {
            setDotIndex((prevIndex) => (prevIndex < 3 ? prevIndex + 1 : 0));
        }, 500);

        return () => clearInterval(timer);
    }, []);


    /**
     * Fetches the active status from the backend.
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

    if (loading) {
        return( 
            <div className='feedcontainer'>
                <div className='noprofiles'>
                    <h1>
                        <FontAwesomeIcon icon={faSpinner} spin /> Loading{Array(dotIndex + 1).join('.')}
                    </h1>
                </div>
            </div>
        );
    }

    if (!isActive) {
        return (
            <div className="feedcontainer">
                <div className='noprofiles'>
                    <h1>Your account is currently deactivated. To regain access to your feed, reactivate your account in the Account page!</h1>
                </div>
            </div>
        )
    }

    return (
        <div className="feedcontainer" {...(disableSwipe ? {} : swipeHandlers)}>
            {profiles && currentProfileIndex < profiles.length ? (
                <>
                    <div className="profilecontainer">
                        <div className="buttoncol-dislike">
                            <button
                                type="button"
                                className="btn dislikebutton shared_like rounded-circle btn-lg"
                                onClick={handleDislike}
                            >
                                <FontAwesomeIcon icon={faTimes} style={{ color: 'white' }} />
                            </button>
                        </div>
                        <div className="profile-info">
                            <h2 className='mt-3'>{currentUser.first_name}</h2>
                            <div className="text-center">
                                <div className="carousel slide mb-2 rounded-5" data-ride="carousel">
                                    <div className="carousel-inner rounded-5">
                                    {currentUser && Array.isArray(currentUser.photos) && currentUser.photos.map((imageUrl, index) => (
                                        <div className={`carousel-item ${index === currentImageIndex ? 'active' : ''}`}  style={{ width: '100%', height: '500px'}} key={index}>
                                            <img src={imageUrl} className="d-block" style={{ height: '100%', width: '100%', objectFit: 'fill' }} alt={`Image ${index}`} />
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
                                            <div className="col-md-12">
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
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            ))}
                        </div>
                        <div className="buttoncol-like">
                            <button
                                type="button"
                                className="btn likebutton shared_like rounded-circle btn-lg"
                                onClick={handleLike}
                            >
                                <FontAwesomeIcon icon={faThumbsUp} style={{ color: 'white' }} />
                            </button>
                        </div>
                    </div>
                </>
            ) : (
                <div className='noprofiles'>
                    <h1>No more profiles to show. Please come back later and try again!</h1>
                </div>
            )}
            {showPopup && (
                        <MatchedPopup trigger={showPopup} setTrigger={setShowPopup}>
                            <div className="popup-content">
                                <h1 style={{ fontWeight: 'bold', fontSize: '100px', marginTop: '100px', textAlign: 'center' }}>
                                    Congratulations!
                                    <div className="type-writer-container">
                                        <Typewriter
                                            text="It's a MATCH!!!"
                                            delay={100}
                                            className="type-writer-text"
                                        />
                                    </div>
                                </h1>
                            </div>
                        </MatchedPopup>
            )}
        </div>
    );
}

export {Feed};
import React, { useState, useEffect } from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faThumbsUp, faBirthdayCake, faGraduationCap, faVenusMars, faUserCheck } from '@fortawesome/free-solid-svg-icons';
import Typewriter from 'react-typewriter-effect';
import axios from 'axios';
import "./ProfilePopup.css";
import Chat from "./chat.jsx";

const ProfilePopup = ({ user, onClose }) => {
    const [currentUser, setCurrentUser] = useState(null);
    const [currentImageIndex, setCurrentImageIndex] = useState(0);
    
    useEffect(() => {
        fetchProfileInfo(user.user_id);
    }, [user]);

    useEffect(() => {
        const handleClickOutside = (event) => {
            const popupContent = document.querySelector('.pop-up-container');
            const unmatchButton = document.querySelector('.unmatch');
            const messageButton = document.querySelector('.message');
            if (popupContent && 
                !popupContent.contains(event.target) && 
                !unmatchButton.contains(event.target)) {
                onClose();
            }
        };

        document.addEventListener('mousedown', handleClickOutside);

        return () => {
            document.removeEventListener('mousedown', handleClickOutside);
        };
    }, [onClose]);

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

    const handleNextImage = () => {
        setCurrentImageIndex((prevIndex) => (prevIndex + 1) % currentUser.photos.length);
    };

    const handlePrevImage = () => {
        setCurrentImageIndex((prevIndex) => (prevIndex - 1 + currentUser.photos.length) % currentUser.photos.length);
    };

    const handleUnmatch = () => {
        axios.post(`/unmatch/${user.user_id}`)
            .then(response => {
                console.log(response.data.message);
                onClose(); 
                window.location.reload(); //to show new matches
            })
            .catch(error => {
                console.error('Error unmatching users:', error);
            });
    };
    
    return (
        <div className='vertical-center'>
            <div className="container pop-up-container profile-pop-up-height">
                {currentUser && (
                        <div className="profile-info-popup">
                            <h2 className='mt-3'>{currentUser.first_name}</h2>
                            <div className="text-center">
                                <div className="carousel slide mb-2 rounded-5" data-ride="carousel" style={{ width: '100%', height: '300px'}}>
                                    <div className="carousel-inner rounded-5" >
                                        {currentUser.photos.map((imageUrl, index) => (
                                            <div className={`carousel-item ${index === currentImageIndex ? 'active' : ''}`} style={{ width: '100%', height: '300px'}} key={index}>
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
                                )}
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
                                <div className='buttons mb-4'>
                                    <button className='unmatch' onClick={handleUnmatch}>Unmatch</button>
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
                    )}
                <div className='chat-div'>
                    {currentUser && <Chat otherUser={currentUser} />}
                </div>
            </div>
        </div>
    );
};

export default ProfilePopup;

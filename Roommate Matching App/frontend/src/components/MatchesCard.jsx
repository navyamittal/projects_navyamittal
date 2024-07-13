import React from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faGraduationCap } from '@fortawesome/free-solid-svg-icons'; 
import "./MatchesCard.css";

const MatchesCard = ({ user, onProfileClick }) => {
    return (
        <div className="matches-card shadow rounded-5 mb-4" onClick={onProfileClick}>
            <img src={user.profile_photo} className="card-img-top" alt="User" />
            <div className="card-body">
                <h5 className="card-title">{user.first_name}</h5>
                <p className="card-text">
                    <FontAwesomeIcon icon={faGraduationCap} /> {user.major}
                </p>
            </div>
        </div>
    );
};

export default MatchesCard;

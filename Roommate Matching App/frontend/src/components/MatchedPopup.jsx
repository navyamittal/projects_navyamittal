import React ,{ useEffect, useState } from 'react';
import './MatchedPopup.css' 
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faUser, faComment} from '@fortawesome/free-solid-svg-icons'; 
import { Link } from 'react-router-dom';

export default function MatchedPopup(props){
    useEffect(() => {
        if (props.trigger) {
            document.body.classList.add('popup-open');
        } else {
            document.body.classList.remove('popup-open');
        }
    }, [props.trigger]);

    return(props.trigger) ? ( //if triggered then return
        <div className = "popup">
            <div className='popup-inner'>
                <button className='btn btn-danger closeButton' onClick={() => {
                    props.setTrigger(false);
                    document.body.classList.remove('popup-open'); // Remove the class when the close button is clicked
                }}>Close</button>
                { props.children }

                <div className="popup-buttons" >
                    <Link to="/matches" className="messageButton" style={{ textDecoration: 'none' }}>
                        <FontAwesomeIcon icon={faComment} /> <br />
                        Chat with Them Now!
                    </Link>
                </div>
            </div>
        </div>
    ) : ""; //if not triggered then return an empty string
}
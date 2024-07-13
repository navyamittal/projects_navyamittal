import './Auth.css';
import './Home.css'
import { Link } from "react-router-dom";
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faHeart, faClipboardList, faComments, faAngleDown } from '@fortawesome/free-solid-svg-icons';
import React, { useState, useEffect } from 'react';


/**
 * Frontend component for home/landing page where users can log in / sign up from.
 * @component
 */
function Home() {

    const [opacity, setOpacity] = useState(1);


    useEffect(() => {
        const handleScroll = () => {
            if (window.scrollY > 100) {
                setOpacity(0);
            } else {
                setOpacity(1);
            }
        };

        window.addEventListener('scroll', handleScroll);

        return () => {
            window.removeEventListener('scroll', handleScroll);
        };
    }, []);

    const handleScrollButton = () => {
        window.scrollTo({
            top: document.body.scrollHeight,
            behavior: 'smooth' 
        });
    };

    return (
    <div className='home-container'>
        <div className='auth-div-home'>
            <div className='auth-background auth-background-home'>
                <div className='auth-overlay auth-overlay-home'>
                        <div className='auth-contents'>
                            <h1 className='home-title'>aggie's house</h1>
                            <div className='mt-5 btn-div-home'>
                                <Link to="/signup" className='btn btn-home'>Sign up</Link>
                                <Link to="/login" className='btn btn-home'>Log in</Link>
                            </div>
                        </div>
                </div>
            </div>
        </div>
        <div className='description-background'>
            <div className='description-overlay'>
                <button className='scroll-button' onClick={handleScrollButton} style={{ opacity: `${opacity}`}}>
                    <FontAwesomeIcon icon={faAngleDown} style={{ color: 'white', fontSize: '60px' }}/>
                </button>
                <h2 className='home-subtitle'>find your roommate</h2>
                <div className='icons-container'>
                    {/* <p>Aggie's House is your one-stop shop for finding an off-campus roommate at Texas A&M!</p> */}
                    <div className="icon-div">
                        <FontAwesomeIcon icon={faClipboardList} style={{ color: '#500000', fontSize: '100px' }} />
                        <h3 className="caption">1. Tell Us More</h3>
                        <div className="details">Fill out your profile info by going through our quick questionnaire - answer what matters to you, feel free to skip what doesn’t!</div>
                    </div>
                    <div className="icon-div">
                        <FontAwesomeIcon icon={faHeart} style={{ color: '#500000', fontSize: '100px' }} />
                        <h3 className="caption">2. Meet Your Match</h3>
                        <div className="details">We’ll show you your most compatible potential matches first in an intuitive, dating app style feed. Mutual likes turn into matches!</div>
                    </div>
                    <div className="icon-div">
                        <FontAwesomeIcon icon={faComments} style={{ color: '#500000', fontSize: '100px' }} />
                        <h3 className="caption">3. Connect In App</h3>
                        <div className="details">Message your matches securely and conveniently within our website!</div>
                    </div>
                </div>
            </div>
        </div>
    </div>
    );
};

export {Home};
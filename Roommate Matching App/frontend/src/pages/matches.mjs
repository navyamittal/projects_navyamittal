import React, { useState, useEffect } from 'react';
import MatchesCard from '../components/MatchesCard.jsx';
import axios from 'axios';
import "./matches.css"
import ProfilePopup from '../components/ProfilePopup.jsx';

/**
 * Frontend component for Matches page.
 * @component
 */
const Matches = () => {
    const [matches, setMatches] = useState([]);
    const [selectedUser, setSelectedUser] = useState(null); 

    useEffect(() => {
        // Fetch matches data from the backend using an API call
        fetchMatches();
    }, []);

    const openProfilePopup = (user) => {
        setSelectedUser(user);
    };

    const closeProfilePopup = () => {
        setSelectedUser(null);
    };

    const fetchMatches = () => {
        axios.get('/getmatches')
        .then(response => {
            // Set the current user with the fetched profile data
            setMatches(response.data.matches);
        })
        .catch(error => {
            console.error('Error fetching matches: ', error);
        });
    };

    return (
        <div className="container navbar-adjust mb-4 pt-10"> {/* Add pt-4 class for top padding */}
            {selectedUser && <ProfilePopup user={selectedUser} onClose={closeProfilePopup} />}
            
            {matches.length === 0 ? (
                <div className="feedcontainer">
                    <div className='noprofiles'>
                        <h1>No matches available at this moment. Continue using the app to see if you can match with anyone!</h1>
                    </div>
                </div>
            ) : (
                <>
                    {/* <h2 className="mb-4">Matches</h2> */}
                    <div className="row row-cols-1 row-cols-md-3 g-4"> {/* Add g-4 class for padding between rows */}
                        {matches.map(match => (
                            <div key={match.user_id} className="col">
                                <MatchesCard user={match} onProfileClick={() => openProfilePopup(match)} />
                            </div>
                        ))}
                    </div>
                </>
            )}
        </div>
    );
};

export default Matches;

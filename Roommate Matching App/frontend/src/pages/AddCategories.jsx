import React, { useState, useEffect } from 'react';
import axios from 'axios';
import './Settings.css';
import { useNavigate } from 'react-router-dom';

/**
 * Frontend component for selecting new categories to add on Account page.
 * @component
 * @param {function} toggleCatView - Function to toggle Account page view between adding and deleting categories or viewing profile.
 */
function AddCategories({toggleCatView}) {
    const [categories, setCategories] = useState(null);
    const [toAdd, setToAdd] = useState([]);

    const navigate = useNavigate();
    
    useEffect(() => {
        fetchEmptyCategories();
    }, []);

    /**
     * Fetches empty categories from the backend.
     * @function
     */
    const fetchEmptyCategories = async () => {        
        axios.get('/settings/get_empty_categories')
            .then(response => {
                setCategories(response.data);
            })
            .catch(error => {
                console.error('Error fetching empty categories:', error);
            });
    };

    /**
     * Handles the change in category selection.
     * @param {string} categoryId - The ID of the category being changed.
     */
    const handleChange = (categoryId) => {
        if (toAdd.includes(categoryId)) {
            // remove if already in toAdd
            const edited = toAdd.filter(id => id !== categoryId);
            setToAdd(edited);
        } else {
            // otherwise add
            setToAdd(prev => ([...prev, categoryId]));
        }
        console.log(toAdd);
    };

    /**
     * Submits selected categories to the next page.
     */
    const submitCategories = () => {
        navigate('/questionnaire2', {state: {categories: toAdd }}); // pass toAdd to questionnaire2
    }

    return (
        <div>
            {categories && Object.keys(categories).length > 0 ? (
                <div>
                    <h3 className='settings-header'>Select Categories to Add</h3>
                    <div className='category-container'>
                        {Object.keys(categories).map((categoryId) => (
                            <button
                                key={categoryId}
                                className='answer-choices'
                                style={{ border: 'none', margin: '5px', padding: '5px', paddingLeft: '15px', paddingRight: '15px',  backgroundColor: toAdd.includes(categoryId) ? '#500000' : '#d3d0d0', color: toAdd.includes(categoryId) ? 'white' : 'black' }}
                                onClick={() => handleChange(categoryId)}
                            >
                                {categories[categoryId]}
                            </button>
                        ))}
                    </div>
                </div>
            ) : (
                <h3 className='settings-header'>There are no categories left to add!</h3>
            )}
            <div className='category-btn-div'>
                <button className='btn categories-btn' onClick={() => toggleCatView(0)}>Go Back</button>
                {categories && Object.keys(categories).length > 0 ? (<button className='btn categories-btn' onClick={() => submitCategories()}>Continue</button>) : null}
            </div>
        </div>
    );
}

export {AddCategories};
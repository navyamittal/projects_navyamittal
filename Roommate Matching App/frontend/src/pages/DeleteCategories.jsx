import React, { useState, useEffect } from 'react';
import axios from 'axios';
import './Settings.css'

/**
 * Frontend component for selecting existing categories to delete on Account page.
 * @component
 * @param {function} toggleCatView - Function to toggle Account page view between adding and deleting categories or viewing profile.
 */
function DeleteCategories({toggleCatView}) {
    const [categories, setCategories] = useState(null);
    const [toDelete, setToDelete] = useState([]);

    useEffect(() => {
        fetchCurrentCategories();
    }, []);

    /**
     * Fetches current categories from the backend.
     */
    const fetchCurrentCategories = async () => {        
        axios.get('/settings/get_current_categories')
            .then(response => {
                setCategories(response.data);
            })
            .catch(error => {
                console.error('Error fetching empty categories:', error);
            });
    };

    /**
     * Handles the change in category selection for deletion.
     * @param {string} categoryId - The ID of the category being changed.
     */
    const handleChange = (categoryId) => {
        if (toDelete.includes(categoryId)) {
            // remove if already in toDelete
            const edited = toDelete.filter(id => id !== categoryId);
            setToDelete(edited);
        } else {
            // otherwise add
            setToDelete(prev => ([...prev, categoryId]));
        }
        console.log(toDelete);
    };

    /**
     * Submits selected categories for deletion to the backend.
     */
    const submitCategories = async () => {
        try {
            await axios.post('/settings/delete_categories', {
                categories: toDelete
            });
            console.log('Answers deleted successfully');
        } catch (error) {
            console.error('Error deleting answers:', error);
        }
    }

    return (
        <div>
            {categories && Object.keys(categories).length > 0 ? (
                <div>
                    <h3 className='settings-header'>Select Categories to Delete</h3>
                    <div className='category-container'>
                        {Object.keys(categories).map((categoryId) => (
                            <button
                                key={categoryId}
                                className='answer-choices'
                                style={{ border: 'none', margin: '5px', padding: '5px', paddingLeft: '15px', paddingRight: '15px',  backgroundColor: toDelete.includes(categoryId) ? '#500000' : '#d3d0d0', color: toDelete.includes(categoryId) ? 'white' : 'black' }}
                                onClick={() => handleChange(categoryId)}
                            >
                                {categories[categoryId]}
                            </button>
                        ))}
                    </div>
                </div>
            ) : (
                <h3 className='settings-header'>There are no categories left to delete!</h3>
            )}
            <div className='category-btn-div'>
                <button className='btn categories-btn' onClick={() => toggleCatView(0)}>Go Back</button>
                {categories && Object.keys(categories).length > 0 ? (<button className='btn categories-btn' onClick={() => {submitCategories(); toggleCatView(0)}}>Submit</button>) : null}
            </div>
        </div>
    );
}

export {DeleteCategories};
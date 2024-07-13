import React, { useState, useEffect } from 'react';
import { Profile } from "./Profile"
import EditProfile from "./EditProfile"
import { AddCategories } from './AddCategories';
import { DeleteCategories } from './DeleteCategories';
import './Settings.css'

/**
 * Frontend component for Account page container. Manages which view is displayed.
 * @component
 */
function Settings(){
    const [shouldEdit, setShouldEdit] = useState(false);
    const [categoryState, setCategoryState] = useState(0); // 1 means add, 2 means delete, 0 means just view profile
    const [category, setCategory] = useState(null);

    /**
     * Toggles the edit view mode.
     * @param {string} value - The category value.
     */
    const toggleEditView = (value) => {
        setShouldEdit(prev => !prev);
        setCategory(value);
    };

    /**
     * Toggles the category view mode.
     * @param {string} value - The category value.
     */
    const toggleCatView = (value) => {
        setCategoryState(value);
    }

    return (
        <div style={{height: '80vh', marginTop: '20vh'}}>
            {/* <h1 className='settings-header'>View/Edit Profile</h1> */}
            {shouldEdit ? (
                <EditProfile toggleEditView={toggleEditView} category={category}></EditProfile>
            ) : (
                categoryState === 1 ? (
                    <AddCategories toggleCatView={toggleCatView}></AddCategories>
                ) : categoryState === 2 ? (
                    <DeleteCategories toggleCatView={toggleCatView}></DeleteCategories>
                ) : (
                    <Profile toggleEditView={toggleEditView} toggleCatView={toggleCatView}></Profile>
                )
            )}
        </div>
    );
}

export {Settings};
import { useState, useEffect } from 'react'
import axios from "axios";
import logo from '../logo.svg';
import './Auth.css';
import './CompleteProfile.css';
import {faImage} from '@fortawesome/free-solid-svg-icons';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { Link, useNavigate } from "react-router-dom";
import ImageUpload from '../components/ImageUpload';

/**
 * Frontend component for filling out basic profile information required by all users.
 * @component
 */
function CompleteProfile() {
    const [first_name, setFirstName] = useState('');
    const [birthday, setBirthday] = useState('');
    const [gender, setGender] = useState('');
    const [major, setMajor] = useState('');
    const [bio, setBio] = useState('');
    const [majorOptions, setMajorOptions] = useState([]);
    const [completeProfileMessage, setCompleteProfileMessage] = useState('');
    const [isVisible, setIsVisible] = useState(false);
    const [imagesUploaded, setImagesUploaded] = useState(false);
    const [formSubmitted, setFormSubmitted] = useState(false);
    const navigate = useNavigate();

    useEffect(() => {
        async function fetchMajorOptions() {
            try {
                const response = await axios.get('/majors');
                if (response.data) {
                    setMajorOptions(response.data);
                }
            } catch (error) {
                console.error('Error fetching major options:', error);
            }
        }

        /**
         * Fetches profile information from the backend.
         */
        async function fetchProfileInfo() {
            try{
                const response = await axios.get('/profile_info');
                const { first_name, birthday, gender, major, bio } = response.data;
                setFirstName(first_name || '');
                setBirthday(birthday || '');
                setGender(gender || '');
                setMajor(major || '');
                setBio(bio || '');
                console.log(birthday)
            } catch (error) {
                console.error('Error fetching profile info: ', error)
            }
        }

        fetchProfileInfo();
        fetchMajorOptions();
    }, []);

    /**
     * Handles form submission.
     * @param {object} e - The form submission event.
     */
    const handleSubmit = async (e) => {
        e.preventDefault();
        try {
            setFormSubmitted(true);
            if (!first_name.trim()) {
                setCompleteProfileMessage('First name is required.');
                return;
            }

            if (!birthday) {
                setCompleteProfileMessage('Birthday is required.');
                return;
            }

            if (!gender) {
                setCompleteProfileMessage('Gender is required.');
                return;
            }

            if (!major) {
                setCompleteProfileMessage('Major is required.');
                return;
            }

            if (!bio.trim()) {
                setCompleteProfileMessage('Bio is required.');
                return;
            }

            if (!imagesUploaded) {
                setCompleteProfileMessage('Must upload at least 1 image.');
                return;
            }

            const profileData = {
                first_name: first_name,
                birthday: birthday,
                gender: gender,
                major: major,
                bio: bio
            };
    
            // Send profile data to the backend
            await axios.post('/profile_info', profileData);
    
            // Redirect to the questionnaire
            
            navigate('/questionnaire');
        } catch (error) {
            setCompleteProfileMessage('Missing Required Info');
            setFormSubmitted(true);
        }
    }

    const handleShowImageUpload = () => {
        setIsVisible(true);
        document.body.classList.add('popup-open');
    };
    
    return (
    <div className='auth-background'>
        <div className='auth-overlay'>
            <div className='auth-container'>
                <div className='complete-profile-contents'>
                    <form onSubmit={handleSubmit}>
                        <h2 className='welcome-text mt-3' style={{padding: '0'}}>Complete Your Profile</h2>
                        <p className='auth-redirect'>Once you complete your profile you will move on to the questionnaire.</p>
                        {formSubmitted && completeProfileMessage && <div className="alert alert-danger">{completeProfileMessage}</div>}
                        <div className='mb-3'>
                            <label>First Name</label>
                            <input type='text' placeholder='First Name' className='form-control' value={first_name} onChange={(e) => setFirstName(e.target.value)}/>
                        </div>
                        <div className='mb-3'>
                            <label>Birthday</label>
                            <input type='date' className='form-control' value={birthday} onChange={(e) => setBirthday(e.target.value)}/>
                        </div>
                        <div className='mb-3'>
                            <label>Gender</label>
                            <select className='form-control' value={gender} onChange={(e) => setGender(e.target.value)}>
                                <option value="">Select Gender</option>
                                <option value="Male">Male</option>
                                <option value="Female">Female</option>
                                <option value="Non-binary">Non-binary</option>
                            </select>
                        </div>
                        <div className='mb-3'>
                            <label>Major</label>
                            <select className='form-control' value={major} onChange={(e) => setMajor(e.target.value)}>
                                <option value="">Select Major</option>
                                {majorOptions.map(option => (
                                    <option key={option} value={option}>{option}</option>
                                ))}
                            </select>
                        </div>
                        <div>
                            <label>Profile Pictures</label>
                            <button type="button" className="upload-image-button mb-3" onClick={handleShowImageUpload}>
                                <FontAwesomeIcon icon={faImage} className="icon" />
                                Upload Images to Show on Your Profile
                            </button>
                            {isVisible && <ImageUpload isVisible={isVisible} setIsVisible={setIsVisible} imagesUploaded={imagesUploaded} setImagesUploaded={setImagesUploaded} setFormSubmitted={setFormSubmitted} formSubmitted={formSubmitted} />}
                        </div>
                        <div className='mb-3'>
                            <label>Bio</label>
                            <textarea placeholder='Tell us about yourself!' className='form-control' maxLength={500} style={{ resize: 'none', height: '150px' }} value={bio} onChange={(e) => setBio(e.target.value)}/>
                            <p>Characters left: {500 - bio.length}</p>
                        </div>
                        <div className='mb-3'>
                            <button type='submit' className='btn btn-maroon'>Save and Continue</button>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>
    );
}

export {CompleteProfile};
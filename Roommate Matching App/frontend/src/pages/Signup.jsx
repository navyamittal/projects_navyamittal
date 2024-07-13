import { useState, useEffect } from 'react'
import axios from "axios";
import logo from '../logo.svg';
import './Auth.css';
import { Link, useNavigate } from "react-router-dom";
import { gapi } from 'gapi-script';
import { GoogleLogin, GoogleOAuthProvider } from '@react-oauth/google';
import {jwtDecode} from 'jwt-decode';

/**
 * Frontend component for signing up to the application.
 * @component
 */
function Signup() {
    const [email, setEmail] = useState('');
    const [password, setPassword] = useState('');
    const [first_name, setFirstName] = useState('');
    const [birthday, setBirthday] = useState('');
    const [gender, setGender] = useState('');
    const [major, setMajor] = useState('');
    const [bio, setBio] = useState('');
    const [majorOptions, setMajorOptions] = useState([]);
    const [signupMessage, setSignupMessage] = useState('');
    const navigate = useNavigate();
    const clientId = process.env.REACT_APP_OAUTH_CLIENT_ID;
   

    // useEffect(() => {
    //     function start() {
    //         gapi.client.init({
    //             clientId: clientId,
    //             scope: ""
    //         });
    //     };
    //     gapi.load('client:auth2', start);
    // }, []);

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

        fetchMajorOptions();
    }, []);

    /**
     * Handles form submission for user signup.
     * @param {Event} e - The submit event.
     * @returns {Promise<void>}
     */
    const handleSubmit = async (e) => {
        e.preventDefault();
        try {
            const response = await axios.post('/signup', { email, password, first_name, birthday, gender, bio, major });
            const profileRes = await axios.get('/profile_status');
            const profileStatus = profileRes.data.profile_status;
            if (profileStatus) {
                // Redirect to complete profile page if profile is incomplete
                navigate('/complete_profile');
            } else {
                // Redirect to questionnaire if profile is complete
                navigate('/questionnaire');
            }
        } catch (error) {
            setSignupMessage('Error signing up, if you already have an account go to log in!');
        }
    };

    /**
     * Handles successful Google authentication response.
     * @param {Object} response - The Google authentication response object.
     * @returns {Promise<void>}
     */
    const onSuccess = async (response) => {
        try {
            // Send Google authentication response to backend
            const accessToken = response.credential;
            const decodedToken = jwtDecode(accessToken);
            const { given_name, email, picture } = decodedToken;
            const res = await axios.post('/gauthorize', {
                given_name: given_name,
                email: email,
                picture: picture
            });
            
            if (res.data.success) {
                // Check profile status after successful authentication
                const profileRes = await axios.get('/profile_status');
                const profileStatus = profileRes.data.profile_status;
                
                if (profileStatus) {
                    // Redirect to complete profile page if profile is incomplete
                    navigate('/complete_profile');
                } else {
                    // Redirect to questionnaire if profile is complete
                    navigate('/questionnaire');
                }
            } else {
                setSignupMessage('Failed to log in with Google');
            }
        } catch (error) {
            console.error('Error signing up with Google:', error);
            setSignupMessage('Failed to sign up with Google');
        }
    };

    /**
     * Handles failure of Google authentication.
     * @param {Object} res - The failure response object.
     */
    const onFailure = (res) => {
        console.log("fail", res)
        setSignupMessage('Error with Google Signup');
    }

    return (
        <GoogleOAuthProvider clientId={clientId}>
            <div className='auth-background'>
                <div className='auth-overlay'>
                    <div className='auth-container'>
                        <Link to="/" className="btn-close exit" aria-label="Close"></Link>
                        <div className='auth-contents-signup'>
                            <form onSubmit={handleSubmit}>
                                <h2 className='welcome-text' style={{ padding: '0' }}>Welcome to Aggie's House!</h2>
                                {signupMessage && <div className="alert alert-danger">{signupMessage}</div>}
                                <div className='mt-5 mb-3'>
                                    <label>Email</label>
                                    <input type='email' placeholder='Email' className='form-control' value={email} onChange={(e) => setEmail(e.target.value)} required />
                                </div>
                                <div className='mb-4'>
                                    <label>Password</label>
                                    <input type='password' placeholder='Password' className='form-control' value={password} onChange={(e) => setPassword(e.target.value)} required />
                                </div>
                                <div className='mb-3'>
                                    <button type='submit' className='btn btn-maroon'>Sign up</button>
                                </div>
                            </form>
                            <p className='or'>OR</p>
                            <p className='auth-redirect'>If you choose to sign up with your TAMU email you will be verified!</p>
                            <div className='other-options'>
                                <div className='mb-3-google-login'>
                                    <GoogleLogin
                                        onSuccess={onSuccess}
                                        onFailure={onFailure}
                                        cookiePolicy={'single_host_origin'}>
                                        Sign up with Google
                                    </GoogleLogin>
                                </div>
                                {/* <div className='mb-5-tamu-login'>
                                    <GoogleLogin
                                        clientId={clientId}
                                        buttonText="Sign in with TAMU email"
                                        onSuccess={onTamuSuccess}
                                        onFailure={onFailure}
                                        cookiePolicy={'single_host_origin'}  
                                    />
                                </div> */}
                            </div>
                            <p className='auth-redirect'>Have an account already? <Link to="/login">Log in</Link></p>
                        </div>
                    </div>
                </div>
            </div>
        </GoogleOAuthProvider>
    );
}

export {Signup};
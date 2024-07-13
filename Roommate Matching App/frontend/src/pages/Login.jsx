import './Auth.css';
import { Link, useNavigate } from "react-router-dom";
import { useState } from 'react';
import axios from 'axios';
import { GoogleLogin, GoogleOAuthProvider } from '@react-oauth/google';
import { useEffect } from 'react';
import { gapi } from 'gapi-script';
import {jwtDecode} from 'jwt-decode';

/**
 * Frontend component for logging in to the application.
 * @component
 */
function Login() {
    const [email, setEmail] = useState('');
    const [password, setPassword] = useState('');
    const [loginMessage, setLoginMessage] = useState('');
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
    
    /**
     * Handles form submission for user login.
     * @param {Event} e - The submit event.
     * @returns {Promise<void>}
     */ 
    const handleSubmit = async (e) => {
        e.preventDefault();
        try {
            const response = await axios.post('/login', { email, password });

            const profileRes = await axios.get('/profile_status');
            const profileStatus = profileRes.data.profile_status;
            
            if (profileStatus) {
                // Redirect to complete profile page if profile is incomplete
                navigate('/complete_profile');
            } else {
                // Redirect to questionnaire if profile is complete
                navigate('/feed');
            }

        } catch (error) {
            setLoginMessage('Invalid email or password');
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
                    navigate('/feed');
                }
            } else {
                setLoginMessage('Failed to log in with Google');
            }
        } catch (error) {
            console.error('Error logging in with Google:', error);
            setLoginMessage('Failed to log in with Google');
        }
    };

    /**
     * Handles failure of Google authentication.
     * @param {Object} res - The failure response object.
     */
    const onFailure = (res) => {
        console.log("fail", res)
    }


    return (
        <GoogleOAuthProvider clientId={clientId}>
            <div className='auth-background'>
                <div className='auth-overlay'>
                    <div className='auth-container'>
                        <Link to="/" className="btn-close exit" aria-label="Close"></Link>
                        <div className='auth-contents'>
                            <form onSubmit={handleSubmit}>
                                <h2 className='welcome-text'>Welcome to Aggie's House!</h2>
                                {loginMessage && <div className="alert alert-danger">{loginMessage}</div>}
                                <div className='mb-3 mt-5'>
                                    <label>Email</label>
                                    <input type='email' placeholder='Email' className='form-control' value={email} onChange={(e) => setEmail(e.target.value)} />
                                </div>
                                <div className='mb-4'>
                                    <label>Password</label>
                                    <input type='password' placeholder='Password' className='form-control' value={password} onChange={(e) => setPassword(e.target.value)} />
                                </div>
                                <div className='mb-3'>
                                    <button type='submit' className='btn btn-maroon'>Log in</button>
                                </div>
                            </form>
                            <p className='or'>OR</p>
                            <div className='mb-3-google-login'>
                                <GoogleLogin
                                    onSuccess={onSuccess}
                                    onError={onFailure}
                                    cookiePolicy={'single_host_origin'}
                                />
                            </div>
                            <p className='auth-redirect'>Don't have an account yet? <Link to="/signup">Sign up</Link></p>
                        </div>
                    </div>
                </div>
            </div>
        </GoogleOAuthProvider>
    );
}

export {Login};

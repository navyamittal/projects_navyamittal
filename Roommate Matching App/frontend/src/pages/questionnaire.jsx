import React, { useState, useEffect } from 'react';
import axios from 'axios';
import { useNavigate } from "react-router-dom";
import './questionnaire.css';
import { Helmet } from 'react-helmet';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faChevronLeft, faExclamationCircle } from '@fortawesome/free-solid-svg-icons';

/**
 * Frontend component for main questionnaire page for new users to fill out.
 * @component
 */
function Questionnaire() {
    const [questions, setQuestions] = useState([]);
    const [currentCategory, setCurrentCategory] = useState(1);
    const [answers, setAnswers] = useState({});
    const [userId, setUserId] = useState(null);
    const [skipError, setSkipError] = useState(false);
    const navigate = useNavigate();

    useEffect(() => {
        fetchQuestionsByCategory(currentCategory);
        fetchUserId();
    }, [currentCategory]);

    useEffect(() => {
        setSkipError(false);
    }, [currentCategory]);

    const fetchUserId = async () => {
        try {
            const response = await axios.get('/get_user_id');
            setUserId(response.data.user_id);
        } catch (error) {
            console.error('Error fetching user ID:', error);
        }
    };

    const ProgressBar = ({ currentCategory }) => {
        const progress = (currentCategory) / 8 * 100;
        return (
            <div className="progress-bar">
                <div className="progress" style={{ width: `${progress}%`, backgroundColor: '#500000' }}></div>
            </div>
        );
    };

    const fetchQuestionsByCategory = async (categoryId) => {
        try {
            const response = await axios.get(`/questionnaire/${categoryId}`);
            const initialAnswers = {};
    
            // Fetch user answers for the current category
            const userAnswersResponse = await axios.get(`/user_answers/${categoryId}`);
            const userAnswersData = userAnswersResponse.data;
    
            response.data.forEach(question => {
                if (question.question_type === "multi select") {
                    // Initialize with previously selected options if available
                    initialAnswers[question.question_id] = userAnswersData[question.question_id] ? userAnswersData[question.question_id] : [];
                } else {
                    initialAnswers[question.question_id] = Array.isArray(userAnswersData[question.question_id]) ? userAnswersData[question.question_id][0] : null;
                }
            });
            
            setAnswers(initialAnswers);
            setQuestions(response.data);
        } catch (error) {
            console.error('Error fetching questions:', error);
        }
    };    

    const handlePrevCategory = async () => {
        if (currentCategory > 1) {
            setCurrentCategory(currentCategory - 1);
        }
    };

    const handleNextCategory = async () => {
        if (currentCategory < 8) {
            if (currentCategory === 1) {
                // Check if all questions in category 1 are answered
                const categoryOneAnswered = Object.values(answers).filter((answer, index) => index < questions.length).every(answer => answer !== null);
                if (!categoryOneAnswered) {
                    setSkipError(true);
                    window.scrollTo(0, 0);
                    return;
                }
            }

            // All questions are answered or if not category 1, continue
            setSkipError(false);
            await saveAnswersToDatabase();
            setCurrentCategory(currentCategory + 1); 
        }
        else {
            await saveAnswersToDatabase();
            navigate('/feed')
        }
    };

    const handleAnswerSelection = (questionId, selectedOption) => {
        // If the selected option is already the current answer, deselect it
        const newAnswer = answers[questionId] === selectedOption ? null : selectedOption;
    
        setAnswers(prevState => ({
            ...prevState,
            [questionId]: newAnswer
        }));
    };

    const saveAnswersToDatabase = async () => {
        try {
            await axios.post('/save_answers', {
                user_id: userId,
                answers: answers
            });
            console.log('Answers saved successfully');
        } catch (error) {
            console.error('Error saving answers:', error);
        }
    };

    return (
        <div>
            <Helmet>
                <link href="https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;500;600;700&display=swap" rel="stylesheet" />
            </Helmet>

            <div className="progress-bar">
                <ProgressBar currentCategory={currentCategory} />
            </div>

            <div className="backBtn-skip-container">
                {currentCategory !== 1 && (
                    <div className="backContainer">
                        <FontAwesomeIcon icon={faChevronLeft} onClick={handlePrevCategory} />
                    </div>
                )}

                {currentCategory !== 1 && (
                    <div className="skipContainer">
                        <p className="skipBtn" onClick={handleNextCategory}>SKIP</p>
                    </div>
                )}

            </div>

            <div className="skip-error">
                {skipError && (
                    <div className="error-content">
                        <FontAwesomeIcon icon={faExclamationCircle} className='error-sign' size='2x' />
                        <p className="skipError">
                            All questions on this page are required.
                        </p>

                    </div>

                )}
            </div>

            <div className="page-container">
                {questions.map((question) => (
                    <div key={question.question_id} className="question-container">
                        <div className="question">
                            <p>{question.question_text}</p>
                        </div>
                        <div className="answer">
                            {question.question_type === "multi select" ? (
                                <div className='answer-choices-container'>
                                    {question.options.map((option, index) => (
                                        <button
                                            key={index}
                                            className='answer-choices'
                                            style={{
                                                backgroundColor: answers[question.question_id].includes(option) ? '#500000' : '#d3d0d0',
                                                color: answers[question.question_id].includes(option) ? 'white' : 'black',
                                                margin: '5px',
                                                padding: '5px',
                                                paddingLeft: '15px',
                                                paddingRight: '15px',
                                                border: 'none'
                                            }}
                                            onClick={() => {
                                                const selectedOptions = [...answers[question.question_id]];
                                                const optionIndex = selectedOptions.indexOf(option);
                                                if (optionIndex === -1) {
                                                    selectedOptions.push(option);
                                                } else {
                                                    selectedOptions.splice(optionIndex, 1);
                                                }
                                                handleAnswerSelection(question.question_id, selectedOptions);
                                            }}
                                        >
                                            {option}
                                        </button>
                                    ))}
                                </div>
                            ) : (
                                <div>
                                    {question.options.map((option, index) => (
                                        <button
                                            key={index}
                                            className='answer-choices'
                                            style={{
                                                border: 'none',
                                                margin: '5px',
                                                padding: '5px',
                                                paddingLeft: '15px',
                                                paddingRight: '15px',
                                                backgroundColor: answers[question.question_id] === option ? '#500000' : '#d3d0d0',
                                                color: answers[question.question_id] === option ? 'white' : 'black'
                                            }}
                                            onClick={() => handleAnswerSelection(question.question_id, option)}
                                        >
                                            {option}
                                        </button>
                                    ))}
                                </div>
                            )}
                        </div>
                    </div>
                ))}

                <div className="nextBtn-container">
                    <button className="nextBtn" onClick={handleNextCategory}>CONTINUE</button>
                </div>
            </div>
        </div>
    );
}

export {Questionnaire};
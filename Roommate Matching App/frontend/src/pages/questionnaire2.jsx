import React, { useState, useEffect } from 'react';
import axios from 'axios';
import Select from 'react-select'; 
import { Link, useNavigate, useLocation } from "react-router-dom";
import './questionnaire.css';
import { Helmet } from 'react-helmet';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faChevronLeft } from '@fortawesome/free-solid-svg-icons';
import { faExclamationCircle } from '@fortawesome/free-solid-svg-icons';

/**
 * Frontend component for modified questionnaire accessible through add categories. Will only go through pages corresponding to desired categories to add.
 * @component
 */
function Questionnaire2() {
    // accessing state info passed thru navigate in addcategories
    const location = useLocation();
    const toAdd = location.state.categories; 

    
    const [questions, setQuestions] = useState([]);
    const [currentCategory, setCurrentCategory] = useState(toAdd[0]); 
    const [currentInd, setCurrentInd] = useState(0); 
    const [answers, setAnswers] = useState({}); 
    const [userId, setUserId] = useState(null); 
    const [skipError, setSkipError] = useState(false); 
    const navigate = useNavigate();


    useEffect(() => {
        setCurrentCategory(toAdd[currentInd]);
    }, [currentInd]);

    useEffect(() => {
        fetchQuestionsByCategory(currentCategory);
        fetchUserId(); 
    }, [currentCategory])

    useEffect(() => {
        setSkipError(false); 
    }, [currentCategory]); 

    /**
     * Fetches the user ID from the backend.
     */
    const fetchUserId = async () => {
        try {
            const response = await axios.get('/get_user_id'); 
            setUserId(response.data.user_id);
        } catch (error) {
            console.error('Error fetching user ID:', error);
        }
    };

    /**
     * Renders a progress bar based on the current index.
     * @returns {JSX.Element} - The progress bar component.
     */
    const ProgressBar = () => {
      const progress = (currentInd + 1) / toAdd.length * 100; 
      return (
          <div className="progress-bar">
              <div className="progress" style={{ width: `${progress}%`, backgroundColor: '#500000' }}></div>
          </div>
      );
    };

    /**
     * Fetches questions and user answers for the specified category.
     * @param {string} categoryId - The ID of the category to fetch questions for.
     */
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

    /**
     * Moves to the previous category if available.
     */
    const handlePrevCategory = async () => {
        if (currentInd > 0) {
            setCurrentInd(currentInd - 1);
        }
    };
    
    /**
     * Moves to the next category if available.
     */
    const handleNextCategory = async () => {
        if (currentInd < toAdd.length - 1) {
            if (currentCategory === 1) {
                // Check if all questions in category 1 are answered
                const categoryOneAnswered = Object.values(answers).filter((answer, index) => index < questions.length).every(answer => answer !== null);
                if (!categoryOneAnswered) {
                    setSkipError(true);
                    window.scrollTo(0, 0);
                    return;
                }
            }
            
            //  all questions are answered or if not category 1, continue
            setSkipError(false);
            await saveAnswersToDatabase();
            setCurrentInd(currentInd + 1); 
        }
        else {
            await saveAnswersToDatabase();
            navigate('/settings')
        }
    };
    

    /**
     * Handles selection of an answer for a question.
     * @param {string} questionId - The ID of the question being answered.
     * @param {string} selectedOption - The selected option for the question.
     */
    const handleAnswerSelection = (questionId, selectedOption) => {
        // If the selected option is already the current answer, deselect it
        const newAnswer = answers[questionId] === selectedOption ? null : selectedOption;
    
        setAnswers(prevState => ({
            ...prevState,
            [questionId]: newAnswer
        }));
    };

    /**
     * Saves user answers to the database.
     */
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
              <ProgressBar/>
            </div>

        {/* disable skip and back for category 1 */}
            <div className="backBtn-skip-container">  
                {currentCategory !==1 && (
                    <div className="backContainer">
                    <FontAwesomeIcon icon={faChevronLeft} onClick={handlePrevCategory} />
                </div>
                )}
                
                
                {currentCategory !==1 && (  
                    <div className="skipContainer">
                        <p className="skipBtn" onClick={handleNextCategory}>SKIP</p>
                    </div>
                )}
                
            </div>

            <div className="skip-error">
                {skipError && (
                    <div className="error-content">
                        <FontAwesomeIcon icon={faExclamationCircle} className='error-sign' size='2x'/>
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

export {Questionnaire2};

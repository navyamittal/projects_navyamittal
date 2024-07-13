import React, { useState, useEffect } from 'react';
import axios from 'axios';
import Select from 'react-select'; // library for dropdown
import './questionnaire.css';
import { Helmet } from 'react-helmet';

/**
 * Frontend component for editing a specific category on Account page - modified questionnaire page view.
 * @component
 * @param {function} toggleEditView - Function to toggle Account page view between viewing and editing.
 * @param category - Specifies string category name to edit.
 */
const EditProfile = ({toggleEditView, category}) => {
    const [questions, setQuestions] = useState([]);
    const [currentCategory, setCurrentCategory] = useState(null); 
    const [answers, setAnswers] = useState({}); 
    const [userId, setUserId] = useState(null); 

    useEffect(() => {
        fetchCategoryId(category); 
    }, []);

    useEffect(() => {
      fetchQuestionsByCategory(currentCategory);
      fetchUserId(); 
    }, [currentCategory]);

    /**
     * Fetches the ID of a given category from the backend.
     * @param {string} category - The category to fetch the ID for.
     */
    const fetchCategoryId = async (category) => {
        try {
            const response = await axios.get(`/settings/get_category_id?category=${category}`);
            setCurrentCategory(response.data.category_id);
        } catch (error) {
            console.error('Error fetching category ID:', error);
        }
    };

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
     * Fetches questions by category from the backend.
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
     * Submits edited answers to the backend.
     */
    const handleSubmit = async () => {
        try {
            await axios.post('/settings/edit_answers', {
                user_id: userId, 
                answers: answers
            });
            console.log('Answers edited successfully');
        } catch (error) {
            console.error('Error editing answers:', error);
        }
    };

    /**
     * Handles the selection of answers to questions.
     * @param {string} questionId - The ID of the question being answered.
     * @param {string} selectedOption - The selected answer option.
     */
    const handleAnswerSelection = (questionId, selectedOption) => {
        // If the selected option is already the current answer, deselect it
        const newAnswer = answers[questionId] === selectedOption ? null : selectedOption;
    
        setAnswers(prevState => ({
            ...prevState,
            [questionId]: newAnswer
        }));
    };
    

    return (
        <div>
            {/* <h1>Questionnaire</h1> */}
            <Helmet>
            <link href="https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;500;600;700&display=swap" rel="stylesheet" />
            </Helmet>
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
                    <button className="nextBtn" onClick={() => {handleSubmit(); toggleEditView(null);}}>Submit</button>
                </div>
            </div>
        </div>
    );
};

export default EditProfile;

import React, { useEffect, useState } from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faUserCircle, faPlus, faTimesCircle, faTimes, faFloppyDisk } from '@fortawesome/free-solid-svg-icons';
import './ImageUpload.css';
import axios from 'axios';
import { Link } from 'react-router-dom';
import FormData from 'form-data'
import { useLocation } from 'react-router-dom';

export default function ImageUpload({ isVisible, setIsVisible, imagesUploaded, setImagesUploaded, formSubmitted, setFormSubmitted }) {
    const [images, setImages] = useState([]);
    const [error, setError] = useState('');
    const location = useLocation();

    setFormSubmitted(false);

    useEffect(() => {
        axios.get('/photos')
            .then(response => {
                setImages(response.data.user_photos);
            })
            .catch(error => {
                console.error("Error fetching images:", error);
            });
    }, []);

    const handleImageUpload = (index) => (event) => {
        const file = event.target.files[0];
        if (file) {
            const newImages = [...images];
            newImages[index] = file;
            setImages(newImages);
        }
    };

    const handleDragStart = (index, event) => {
        event.dataTransfer.setData('text/plain', index.toString());
    };

    const handleDrop = (index, event) => {
        event.preventDefault();
        const draggedIndex = parseInt(event.dataTransfer.getData('text/plain'));
        if (draggedIndex !== index) {
            const newImages = [...images];
            const temp = newImages[index];
            newImages[index] = newImages[draggedIndex];
            newImages[draggedIndex] = temp;
            setImages(newImages);
        }
    };

    const allowDrop = (event) => {
        event.preventDefault();
    };

    const removeImage = (index) => {
        const newImages = [...images];
        newImages[index] = null;
        setImages(newImages);
    };

    const handleImgUpload = () => {
        const filteredImages = images.filter(image => image !== null && image !== undefined);
        const formData = new FormData();

        if (filteredImages.length === 0) {
            setError('Please upload at least one image.');
            return; // Exit the function if no images are uploaded
        }

        filteredImages.forEach((image, index) => {
            if (typeof image === 'string') {
                formData.append('urls[]', image);
                formData.append('urlIndexes[]', index);
                console.log(index)
            }
            else if (image instanceof File) {
                formData.append(`images[]`, image, image.name);
                formData.append('imageIndexes[]', index);
            }
        });
        
        axios.post('/photos/upload', formData)
        .then(response => {
            console.log('Images uploaded successfully:', response);
            setImagesUploaded(true);
            if (location.pathname === '/settings') {
                window.location.reload();
            }
            else{
                setIsVisible(false);
                setFormSubmitted(false);
                document.body.classList.remove('popup-open');
            }
        })
        .catch(error => {
            setError('Failed to upload images, please try again later.', error);
        });
    };

    const handleClose = () => {
        setIsVisible(false);
        setFormSubmitted(false)
        document.body.classList.remove('popup-open');
        
    };

    const renderUploadSlots = () => {
        const slots = [];
        for (let i = 0; i < 6; i++) {
            slots.push(
                <div
                    key={i}
                    className={`col-md-4 col-sm-6 rounded ${i === 0 ? 'profile-picture' : ''}`}
                    onDrop={(event) => handleDrop(i, event)}
                    onDragOver={(event) => allowDrop(event)}
                >
                    {/* Conditional rendering for image or placeholder */}
                    {images[i] && typeof images[i] === 'string' ? (
                        <div
                            className="upload-image-container mb-4"
                            draggable={true}
                            onDragStart={(event) => handleDragStart(i, event)}
                            style={{ cursor: 'grab' }}
                        >
                            <img
                                src={images[i]}
                                alt={`Image ${i}`}
                                onClick={() => document.getElementById(`file-input-${i}`).click()}
                            />
                            <div className="remove-icon" onClick={() => removeImage(i)}>
                                <FontAwesomeIcon icon={faTimesCircle} size="lg" />
                            </div>
                        </div>
                    ) : images[i] instanceof File ? (
                        <div
                            className="upload-image-container mb-4"
                            draggable={true}
                            onDragStart={(event) => handleDragStart(i, event)}
                            style={{ cursor: 'grab' }}
                        >
                            <img
                                src={URL.createObjectURL(images[i])}
                                alt={`Image ${i}`}
                                onClick={() => document.getElementById(`file-input-${i}`).click()}
                            />
                            {i === 0 && <div className="ribbon rounded-top">Main Profile Picture</div>}
                            <div className="remove-icon" onClick={() => removeImage(i)}>
                                <FontAwesomeIcon icon={faTimesCircle} size="lg" />
                            </div>
                        </div>
                    ) : (
                        <label htmlFor={`file-input-${i}`} className="upload-placeholder mb-4">
                            {i === 0 ? (
                                <FontAwesomeIcon icon={faUserCircle} size="2x" />
                            ) : (
                                <FontAwesomeIcon icon={faPlus} size="2x" />
                            )}
                        </label>
                    )}
                    <input
                        key={images[i] ? (typeof images[i] === 'string' ? images[i] : images[i].name) : `input-${i}`}
                        id={`file-input-${i}`}
                        type="file"
                        accept="image/*"
                        onChange={handleImageUpload(i)}
                        style={{ display: 'none' }}
                    />
                </div>
            );
        }
    
        const firstRow = slots.slice(0, 3);
        const secondRow = slots.slice(3, 6);
    
        return (
            <>
                <div className="row slots-row">
                    {firstRow}
                </div>
                <div className="row slots-row">
                    {secondRow}
                </div>
            </>
        );
    };
    
    return (
        <>
        {isVisible && (
        <div className='vertical-center'>
            <div className="container pop-up-container">
                <button className="btn-close exit" aria-label="Close" onClick={handleClose}></button>
                <div className="text-center mt-4 mb-4">
                    <h2>Upload at least 1 image to show on your profile, up to 6 images.</h2>
                    <h3>Drag and drop images to reorder them.</h3>
                </div>
                {error && <div className="alert alert-danger">{error}</div>}
                <div className="row justify-content-center ml-4 mr-4">
                    {renderUploadSlots()}
                </div>
                <div className="text-center mt-4">
                        <button type="button" className="btn btn-success submit-button mb-4" onClick={handleImgUpload}>
                            Save Images <FontAwesomeIcon icon={faFloppyDisk} />
                        </button>
                    </div>
            </div>
        </div>)}
        </>
        );
}
// Import the necessary libraries
import React from 'react';

// Initialize Socket.IO client
//const socket = io();

// Button component
const JoinRoomButton = (socketInstance ) => {
    const joinRoom = () => {
        const roomName = prompt('Enter the room name to join:');
        if (roomName.trim() !== '') {
            socketInstance.emit('joinRoom', roomName); // Emit a 'joinRoom' event with the room name
        } else {
            alert('Please enter a valid room name.');
        }
    };

    return (
        <button onClick={joinRoom}>Join Room</button>
    );
};

export default JoinRoomButton;

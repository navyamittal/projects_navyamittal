import React from 'react';
import "./ChatHeader.css"

const ChatHeader = ({ user }) => {
  return (
    <div className="chat-header">
      <img src={user.profile_photo} alt="Chat header" className="profile-photo" />
      <h2 className="user-name">{user.first_name}</h2>
    </div>
  );
};

export default ChatHeader;
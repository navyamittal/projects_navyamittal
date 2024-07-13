import axios from "axios";
import { useEffect, useState } from "react";
import "./conversation.css";

import ChatHeader from "./ChatHeader";

export default function Conversation({ user, socket, handleUserClick }) {
  const handleClick = async () => {

    // if (socket) {
    //   // Use the socket instance directly
    //   socket.emit("joinRoom", { userId: user.user_id });
    //   console.log("Joined room for user:", user.user_id);
    // } else {
    //   console.error("Socket not initialized");
    // }

    // handleUserClick(user.user_id);
    
  };

  return (
    <div className="conversation" onClick={handleClick}>
      <img
        className="conversationImg"
        src={user.profile_photo}
        alt="Profile Picture of User"
      />
      <span className="conversationName">{user.first_name}</span>
    </div>
  );
}

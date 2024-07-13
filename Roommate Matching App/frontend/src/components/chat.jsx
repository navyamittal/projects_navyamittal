import React, { useState, useEffect } from 'react';
import Navbar from "./Navbar";
import axios from 'axios';
// import { ChatEngine, getOrCreateChat, ChatHeader } from 'react-chat-engine';
import { ChatEngine, ChatEngineWrapper, ChatList, Socket, ChatFeed, ChatSocket} from 'react-chat-engine';
import './chat-comp.css'

function Chat({ otherUser }) {
  const [chatUser, setChatUser] = useState(null);
  const projectId = process.env.REACT_APP_CHAT_ENGINE_PROJECT_ID;
  const [chatCreds, setChatCreds] = useState(null);
  const [chat, setChat] = useState(null);

  useEffect(() => {
    fetchUser();
  }, []);

  const fetchUser = () => {
    axios.get('/getcurrentuser')
      .then(response => {
        setChatUser(response.data.user);
        setChatCreds({
          projectID: projectId,
          userName: response.data.user.email,
          userSecret: response.data.user.password
        });
      })
      .catch(error => {
        console.error('Error fetching users: ', error);
      });
  };

  useEffect(() => {
    const createChat = async () => {
      if (chatUser && otherUser && chatCreds) {
        try {
          const chat = await getOrCreateChat(chatUser, otherUser);
          setChat(chat); // Update the chat state after it's created
        } catch (error) {
          // Handle any errors that occur during chat creation
          console.error('Error creating chat:', error);
        }
      }
    };
  
    createChat();
  }, [chatUser, chatCreds, otherUser]);

  async function getOrCreateChat(chatUser, otherUser) {
    try {
      const projectId = process.env.REACT_APP_CHAT_ENGINE_PROJECT_ID;
      const userName = chatUser.email;
      const userSecret = chatUser.password;
  
      const headers = {
        "Project-ID": projectId,
        "User-Name": userName,
        "User-Secret": userSecret
      };
  
      const data = {
        "usernames": [otherUser.email, chatUser.email],
        "is_direct_chat": true
      };
  
      const response = await axios.put(
        'https://api.chatengine.io/chats/',
        data,
        { headers }
      );
  
      return response.data;
    } catch (error) {
      console.log('Get or create chat error', error);
      throw error;
    }
  }

  if (!chatUser || !chatUser.email || !chatUser.password || !chat) {
    return <div>Loading user data...</div>;
  }

  return (
    <>
      <div className='chat-container'>
        <ChatEngineWrapper
        
        height='75vh'>
            <Socket 
                projectID={projectId}
                userName={chatUser.email}
                userSecret={chatUser.password}
                offset={-5}
            />
          <ChatFeed 
            activeChat={chat.id} 
            renderChatHeader={() => (
              null
            )}
           />
        </ChatEngineWrapper>
        {/* <ChatEngine
            projectID={projectId}
            userName={chatUser.email}
            userSecret={chatUser.password}
            offset={-5}
            height='75vh'
        /> */}
      </div>
    </>
  );
}

export default Chat;
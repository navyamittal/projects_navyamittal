import { useEffect, useState } from "react";
import Message from "./message";

export default function WebSocketCall({ socket }) {
  const [message, setMessage] = useState("");
  const [messages, setMessages] = useState([]);

  const handleText = (e) => {
    const inputMessage = e.target.value;
    setMessage(inputMessage);
  };

  const handleSubmit = () => {
    if (!message) {
      return;
    }
    socket.emit("data", message);
    setMessage("");
  };

  // Event listener for incoming messages
  useEffect(() => {
    socket.on("data", (data) => {
      setMessages((prevMessages) => [...prevMessages, data.data]);
    });

    return () => {
      socket.off("data"); // Clean up the event listener when component unmounts
    };
  }, [socket]);

  // Event listener for disconnection
  useEffect(() => {
    socket.on("disconnect", (reason) => {
      console.log("Disconnected from server:", reason);
    });

    return () => {
      socket.off("disconnect");
    };
  }, [socket]);

  return (
    <div>
      <h2>WebSocket Communication</h2>
      <input type="text" value={message} onChange={handleText} />
      <button onClick={handleSubmit}>submit</button>
      {/* <ul>
        {messages.map((message, ind) => {
          return <li key={ind}>{message}</li>;
        })}
      </ul> */}

      {/* Replace the <ul> element with the Message components */}
  {messages.map((message, ind) => (
    <Message key={ind} message={message} own={true} />
  ))}
    </div>
  );
}
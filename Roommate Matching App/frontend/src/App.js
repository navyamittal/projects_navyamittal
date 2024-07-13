import "bootstrap/dist/css/bootstrap.min.css"
import './App.css';
import { BrowserRouter, Routes, Route} from "react-router-dom";
import React, { useEffect } from 'react';

import Navbar from "./components/Navbar";

import Layout from "./pages/Layout";
import { Home } from "./pages/Home";
import { Login } from "./pages/Login";
import { Signup } from "./pages/Signup";

import { Feed } from "./pages/feed";
import Matches from "./pages/matches";
import { Settings } from "./pages/settings";

import ImageUpload from "./components/ImageUpload";

import { Questionnaire } from "./pages/questionnaire";
import { CompleteProfile } from "./pages/CompleteProfile";
import { Questionnaire2 } from "./pages/questionnaire2";

export default function App() {
  useEffect(() => {
    document.title = "Aggie's House"; // Set the title when the component mounts
  }, []);

  return (
    <BrowserRouter>
      <div>
        <Routes>
          {/* Define routes without Navbar */}
          <Route path="/" element={<Home />} />
          <Route path="/login" element={<Login />} />
          <Route path="/signup" element={<Signup />} />
          <Route path='/feed' element={<><Navbar/><Feed/></>}/> 
          <Route path='/matches' element={<><Navbar/><Matches/></>}/> 
          <Route path='/settings' element={<><Navbar/><Settings/></>}/>
          <Route path="/questionnaire" element={<Questionnaire />} />
          <Route path="/complete_profile" element={<CompleteProfile />} />
          <Route path="/questionnaire2" element={<Questionnaire2 />} />
          <Route path="/imageupload" element={<ImageUpload />} /> {/* remove later probably, here for testing */}
        </Routes>
      </div>
    </BrowserRouter>
  );
}


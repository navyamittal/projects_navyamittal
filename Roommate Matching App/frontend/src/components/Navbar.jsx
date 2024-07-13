import "./Navbar.css"
import axios from 'axios';
import { NavLink, useNavigate, useLocation } from 'react-router-dom';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { faHome, faHeart, faComment, faCog, faUser, faSignOutAlt } from '@fortawesome/free-solid-svg-icons'; 

export default function Navbar(){
    const navigate = useNavigate();
    const location = useLocation();

    const handleLogout = async (e) => {
        e.preventDefault();
        try {
            const response = await axios.post('/logout');
            navigate('/')
        } catch (error) {
            navigate('/')
        }
    };

    return (
        <nav className="nav">
            <div className="title">
                aggie's house
                <button onClick={handleLogout} className="btn btn-logout btn-danger">
                    <FontAwesomeIcon icon={faSignOutAlt} /> Logout
                </button>
            </div>
            <ul>
                <li>
                    <NavLink to="/feed" activeClassName="active">
                        <FontAwesomeIcon icon={faHome} /> Feed
                    </NavLink>
                </li>
                <li>
                    <NavLink to="/matches" activeClassName="active">
                        <FontAwesomeIcon icon={faHeart} /> Matches
                    </NavLink>
                </li>
                <li>
                    <NavLink to="/settings" activeClassName="active">
                        <FontAwesomeIcon icon={faUser} /> Account
                    </NavLink>
                </li>
            </ul> 
        </nav>
    );
}
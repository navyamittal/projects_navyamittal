import logo from '../logo.svg';
import { Outlet, Link } from "react-router-dom";


export default function Layout() {

    return (
    <div className="App">

        <img src={logo} className="App-logo" alt="logo" />
        <Outlet />
    </div>
    );
}
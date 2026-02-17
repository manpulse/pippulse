import { Link } from "react-router-dom";
import logo from "../assets/logo.png";
import { FiActivity } from "react-icons/fi";
import { FaRegMoon } from "react-icons/fa";

const Header = () => {
  return (
    <div className="bg-white dark:bg-black justify-center h-16 flex items-center px-6 w-screen">
      <div className="w-20 ">
        <FiActivity className="w-20 " color="Blue" size={35} />
      </div>

      <ul className="flex text-xl font-mono  gap-6 list-none items-center ml-auto">
        
        <li>
          <Link to="/" className="text-black dark:text-white">
            Home
          </Link>
        </li>
        <li>
          <Link to="/login" className="text-black dark:text-white">
            Login
          </Link>
        </li>
        <li>
          <Link to="/register" className="text-black dark:text-white">
            Register
          </Link>
        </li>
        <li>
            <div className=""><FaRegMoon className="cursor-pointer" color="white" size={20} /></div>
        </li>
      </ul>
    </div>
  );
};

export default Header;
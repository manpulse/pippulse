import { Routes, Route, Link } from "react-router-dom";
import Login from "./pages/Login.jsx";
import Register from "./pages/Register.jsx";
import Hero from "./pages/Hero.jsx";
import Header from "./Components/Header.jsx";


function App() {
  return (
     <div className="min-h-screen bg-white text-black dark:bg-black dark:text-white ">
      

      <Routes>
        
        <Route path="/" element={<Hero />} />
        
        <Route path="/login" element={<Login />} />
        <Route path="/register" element={<Register />} />
      </Routes>
    </div>
  );
}

export default App;
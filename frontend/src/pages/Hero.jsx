import React, { useState, useEffect } from "react";
import Header from "../Components/Header";
import { GridBackgroundDemo } from "../Components/Grid";
import GetStarted from "../Components/GetStarted";
import Services from "./Services";

const sections = [
  {
    id: "hero",
    content: (
      <div className="absolute inset-0 flex flex-col items-center justify-center text-center px-4">
        <h1 className="text-7xl text-white font-bold">
          Welcome to XXXXX Where{" "}
          <span className="text-blue-500">Health Meets Innovation</span>
        </h1>
        <p className=" text-gray-300 mt-4 break-words max-w-2xl text-3xl">
          Monitor and manage your health with our comprehensive dashboard. Get insights, track progress, and stay informed about your well-being.
        </p>
        
        <GetStarted title="Get Started" />
      </div>
    ),
    color: "bg-black",
  },
  {
    id: "services",
    content: <Services />,
    color: "bg-gray-900",
  },
  // You can add more sections if needed
];

export default function Hero() {
  const [activeSection, setActiveSection] = useState(0);
  const [scrolling, setScrolling] = useState(false);

  // Handle mouse wheel scrolling
  const handleWheel = (e) => {
    e.preventDefault();
    if (scrolling) return; // throttle

    setScrolling(true);
    if (e.deltaY > 0 && activeSection < sections.length - 1) {
      setActiveSection(activeSection + 1);
    } else if (e.deltaY < 0 && activeSection > 0) {
      setActiveSection(activeSection - 1);
    }

    setTimeout(() => setScrolling(false), 600);
  };

  // Optional: handle arrow keys
  const handleKeyDown = (e) => {
    if (scrolling) return;
    if (e.key === "ArrowDown" && activeSection < sections.length - 1) {
      setScrolling(true);
      setActiveSection(activeSection + 1);
      setTimeout(() => setScrolling(false), 600);
    }
    if (e.key === "ArrowUp" && activeSection > 0) {
      setScrolling(true);
      setActiveSection(activeSection - 1);
      setTimeout(() => setScrolling(false), 600);
    }
  };

  useEffect(() => {
    window.addEventListener("keydown", handleKeyDown);
    return () => window.removeEventListener("keydown", handleKeyDown);
  }, [activeSection, scrolling]);

  return (
    <div className="relative h-screen overflow-hidden" onWheel={handleWheel}>
      {/* Sections */}
      <div
        className="transition-transform duration-700 ease-in-out"
        style={{ transform: `translateY(-${activeSection * 100}vh)` }}
      >
        {sections.map((sec, idx) => (
          <div
            key={sec.id}
            className={`h-screen w-screen relative ${sec.color}`}
          >
            {/* Keep Header and Grid only on first section */}
            {idx === 0 && <GridBackgroundDemo />}
            {idx === 0 && (
              <div className="absolute top-0 left-0 w-full">
                <Header />
              </div>
            )}

            {sec.content}
          </div>
        ))}
      </div>

      {/* Dots Navigation */}
      <div className="fixed top-1/2 right-6 transform -translate-y-1/2 flex flex-col gap-4 z-50">
        {sections.map((_, idx) => (
          <div
            key={idx}
            className={`w-4 h-4 rounded-full cursor-pointer transition-all ${
              activeSection === idx ? "bg-white scale-125" : "bg-gray-400"
            }`}
            onClick={() => setActiveSection(idx)}
          ></div>
        ))}
      </div>
    </div>
  );
}
import React from 'react'
import Cards from '../Components/Cards'
import { CiMonitor } from "react-icons/ci";
import { RiGeminiLine } from "react-icons/ri";
import { FaUserDoctor } from "react-icons/fa6";

import { GridBackgroundDemo } from "../Components/Grid";
const Services = () => {
  return (
    <div className="relative min-h-screen h-screen bg-black">
      {/* Grid background */}
      <GridBackgroundDemo />
      <div className="absolute inset-0 flex flex-col items-center justify-center text-center px-4">
        <h1 className="text-5xl text-white font-bold">
          Features
        </h1>
        <div className="mt-8 flex flex-row items-center justify-center gap-x-7">
          <Cards title="Health Monitoring Dashboard"  description="Comprehensive dashboard to monitor and manage your health effectively."  logo={<CiMonitor size={100}></CiMonitor>}/>
           <Cards title="Send to doctor"  description="doctor can see and manage your health data."  logo={<FaUserDoctor size={100}/>}
/>
          <Cards title="Gemini flash" logo={<RiGeminiLine size={90}></RiGeminiLine>
} description="AI-powered insights and assistance for health data analysis." />
        </div>
      </div>
    </div>
  )
}

export default Services
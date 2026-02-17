import { cn } from "../lib/utils";
import React from "react";

export function GridBackgroundDemo() {
  return (
    <div
      className="relative flex h-screen w-full items-center justify-center bg-white dark:bg-black">
      <div
  className={cn(
    "absolute inset-0",
    "[background-size:200px_200px]","opacity-30",
    "[background-image:linear-gradient(to_right,#e4e4e7_1px,transparent_1px),linear-gradient(to_bottom,#e4e4e7_1px,transparent_1px)]",
    "dark:[background-image:linear-gradient(to_right,#262626_1px,transparent_1px),linear-gradient(to_bottom,#262626_1px,transparent_1px)]"
  )}
 />
      {/* Radial gradient for the container to give a faded look */}
      
    </div>
    
  );
}

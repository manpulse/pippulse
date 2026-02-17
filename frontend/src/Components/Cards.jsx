import React from 'react'

const Cards = ({title, description,logo}) => {
  return (
    <div className="bg-white dark:bg-black p-6 rounded-lg flex flex-col items-center justify-center">
      <div>
        {logo}
      </div>
      <div className="flex p-4 flex-col justify-center items-center">
        <h3 className="text-xl font-bold text-gray-900 dark:text-white">{title}</h3>
        <p className="mt-2 text-gray-600 dark:text-gray-300">{description}</p>
      </div>
    </div>
  )
}

export default Cards
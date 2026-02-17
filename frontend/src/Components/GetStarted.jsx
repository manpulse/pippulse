import React from 'react'

const GetStarted = ({title}) => {
  return (
    <div className="text-2xl font-bold dark:text-white mt-6">
        <button className='px-6 py-3 bg-transparent border-white border-2 text-white rounded-lg hover:bg-blue-700 hover:border-none transition duration-600'>
            {title}
        </button>
    </div>
  )
}

export default GetStarted
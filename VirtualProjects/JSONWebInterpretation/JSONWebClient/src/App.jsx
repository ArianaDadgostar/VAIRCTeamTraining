import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from './assets/vite.svg'
import heroImg from './assets/hero.png'
import './App.css'

function MyButton() {
  return (
    <button>
      I'm a button
    </button>
  );
}

export default function Application()
{
  const[apiResponse, updateAPIResponse] = useState(null)
  const Camera = async() => {
    const response = await fetch("http://localhost:5173/Interpreter/Camera", { 
      method: "POST",
      headers: { "Content-Type": "application/json",
                 "Accept" : "image/jpeg"}, // FIX "image" type!!
      body: JSON.stringify({ key: value })
    })
    const image = URL.createObjectURL(await response().blob())
    updateAPIResponse(image);
  }

  return (
    <div>
        <button onClick={Camera}>Camera</button>
        <scrollbars>exposure</scrollbars>
        {apiResponse && <img src={apiResponse} alt="result" />}
    </div>
  );
}

function MyApp() {
  return (
    <div>
      <h1>Welcome to my app</h1>
      <MyButton />
    </div>
  );
}

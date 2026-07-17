import { useState } from 'react'
import './App.css'

function Scrollbars({ exposure, setExposure }) {  // accept as props
  return (
    <div>
      <input
        type="range"
        min={0}
        max={100}
        value={exposure}
        onChange={(e) => setExposure(e.target.value)}
      />
      <label>Exposure: {exposure}</label>
    </div>
  );
}

export default function Application() {
  const [apiResponse, updateAPIResponse] = useState(null);
  const [exposure, setExposure] = useState(50);

  const Camera = async () => {
    const response = await fetch("http://localhost:5173/Interpreter/Camera", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
        "Accept": "image/jpeg"
      },
      body: JSON.stringify({ key: exposure })  // use state directly
    });
    const image = URL.createObjectURL(await response.blob());  // fixed response()
    updateAPIResponse(image);
  };

  return (
    <div>
      <button onClick={Camera}>Camera</button>
      <Scrollbars exposure={exposure} setExposure={setExposure} />  {/* pass props */}
      {apiResponse && <img src={apiResponse} alt="result" />}
    </div>
  );
}
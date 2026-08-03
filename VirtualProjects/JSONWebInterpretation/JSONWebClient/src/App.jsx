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
  try {
    const response = await fetch("http://localhost:5112/Interpreter/Camera", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ key: exposure })
    });

    console.log("Status:", response.status);

    if (!response.ok) {
      console.error("Request failed:", response.status, await response.text());
      return;
    }

    const blob = await response.blob();
    console.log("Content-Type:", blob.type, "Size:", blob.size);

    updateAPIResponse(URL.createObjectURL(blob));
  } catch (err) {
    console.error("Fetch error:", err);
  }
};

  return (
    <div>
      <button onClick={Camera}>Camera</button>
      <Scrollbars exposure={exposure} setExposure={setExposure} />  {/* pass props */}
      {apiResponse && <img src={apiResponse} alt="result" />}
    </div>
  );
}
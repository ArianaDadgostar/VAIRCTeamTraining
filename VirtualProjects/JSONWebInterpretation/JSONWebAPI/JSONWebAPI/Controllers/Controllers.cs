using Microsoft.AspNetCore.Mvc;
using OpenCvSharp;
using JSONWebAPI;

namespace JSONWebAPI.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class InterpreterController : ControllerBase
    {
        [HttpPost("Camera")]
        public IActionResult Camera([FromBody] CameraRequest request)
        {
            string file = $"{{\"Camera\": {{\"exp\": {request.key}}},}}";
            Mat frame = JSONWebAPI.Program.MacCameraFeed(file);

            if (frame == null || frame.Empty())
                return StatusCode(500, "Could not capture frame");

            Cv2.ImEncode(".jpg", frame, out byte[] jpegBytes);
            return File(jpegBytes, "image/jpeg");
        }
    }

    public class CameraRequest
    {
        public int key { get; set; }
    }
}
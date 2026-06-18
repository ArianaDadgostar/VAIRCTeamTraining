using Microsoft.AspNetCore.Mvc;
using OpenCvSharp;

namespace JSONWebAPI.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class InterpreterController : ControllerBase
    {
        static int count = 0;

        [HttpGet("Camera")]
        public VideoCapture Camera()
        {
            using (var capture = new VideoCapture(0)){
                if (!capture.IsOpened()){
                    Console.WriteLine("Error: Camera could not be opened.");
                    return capture;
                }

                using (var frame = new Mat()){
                    while (true){
                        capture.Read(frame);

                        return capture;

                        if (frame.Empty()) break;
                        Cv2.ImShow("Mac Camera Feed", frame);
                        if(Cv2.WaitKey(1) == 27) break;
                    }
                }
            }
            return default;
        }
    }
}
using Microsoft.AspNetCore.Mvc;
using OpenCvSharp;
using JSONWebAPI;

namespace JSONWebAPI.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class InterpreterController : ControllerBase
    {
        static int count = 0;

        [HttpGet("Camera")]
        public Mat Camera(int exp)
        {
            string file = $"{{\"Camera\": {{\"exp\": {exp}}}}}";
            return JSONWebAPI.Program.MacCameraFeed(file);
        }
    }
}
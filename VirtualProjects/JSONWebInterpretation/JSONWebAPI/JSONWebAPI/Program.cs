// See https://aka.ms/new-console-template for more information

using System.ComponentModel.Design.Serialization;

using System;
using System.Security.Cryptography;
using Microsoft.VisualBasic;

using System.Reflection.Metadata.Ecma335;
using System.Runtime.Versioning;
using OpenCvSharp;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
// Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();
builder.Services.AddControllers();

var app = builder.Build();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseHttpsRedirection();

var summaries = new[]
{
    "Freezing", "Bracing", "Chilly", "Cool", "Mild", "Warm", "Balmy", "Hot", "Sweltering", "Scorching"
};

app.MapGet("/weatherforecast", () =>
{
    var forecast =  Enumerable.Range(1, 5).Select(index =>
        new WeatherForecast
        (
            DateOnly.FromDateTime(DateTime.Now.AddDays(index)),
            Random.Shared.Next(-20, 55),
            summaries[Random.Shared.Next(summaries.Length)]
        ))
        .ToArray();
    return forecast;
})
.WithName("GetWeatherForecast")
.WithOpenApi();

app.MapControllers();

app.Run();

record WeatherForecast(DateOnly Date, int TemperatureC, string? Summary)
{
    public int TemperatureF => 32 + (int)(TemperatureC / 0.5556);
}

namespace JSONWebAPI
{
    class Program
    {
        public class IComputerVision {}

        // Capture
        public class Camera           : IComputerVision { public int    exp        { get; set; } }

        // Color
        public class ConvertColor     : IComputerVision { public string from       { get; set; } public string to         { get; set; } }
        public class Grayscale        : IComputerVision { }
        public class Threshold        : IComputerVision { public double thresh     { get; set; } public double maxval     { get; set; } public string type { get; set; } }
        public class AdaptiveThresh   : IComputerVision { public double maxval     { get; set; } public string method     { get; set; } public string type { get; set; } public int blockSize { get; set; } public double C { get; set; } }
        public class InRange          : IComputerVision { public int    lh         { get; set; } public int    ls         { get; set; } public int    lv  { get; set; } public int uh { get; set; } public int us { get; set; } public int uv { get; set; } }
        public class EqualizeHist     : IComputerVision { }

        // Blur
        public class GaussianBlur     : IComputerVision { public int    ksize      { get; set; } public double sigmaX    { get; set; } }
        public class MedianBlur       : IComputerVision { public int    ksize      { get; set; } }
        public class BilateralFilter  : IComputerVision { public int    d          { get; set; } public double sigmaColor { get; set; } public double sigmaSpace { get; set; } }
        public class Blur             : IComputerVision { public int    ksize      { get; set; } }

        // Morphology
        public class Erode            : IComputerVision { public int    ksize      { get; set; } public int    iterations { get; set; } }
        public class Dilate           : IComputerVision { public int    ksize      { get; set; } public int    iterations { get; set; } }
        public class MorphOpen        : IComputerVision { public int    ksize      { get; set; } }
        public class MorphClose       : IComputerVision { public int    ksize      { get; set; } }
        public class MorphGradient    : IComputerVision { public int    ksize      { get; set; } }

        // Edge
        public class Canny            : IComputerVision { public double thresh1    { get; set; } public double thresh2    { get; set; } }
        public class Sobel            : IComputerVision { public int    dx         { get; set; } public int    dy         { get; set; } public int ksize { get; set; } }
        public class Laplacian        : IComputerVision { public int    ksize      { get; set; } }

        // Geometry
        public class Resize           : IComputerVision { public int    width      { get; set; } public int    height     { get; set; } }
        public class Rotate           : IComputerVision { public double angle      { get; set; } }
        public class Flip             : IComputerVision { public int    flipCode   { get; set; } }
        public class Crop             : IComputerVision { public int    x          { get; set; } public int    y          { get; set; } public int width { get; set; } public int height { get; set; } }
        public class Warp             : IComputerVision { public double m00        { get; set; } public double m01        { get; set; } public double m02 { get; set; } public double m10 { get; set; } public double m11 { get; set; } public double m12 { get; set; } }

        // Drawing
        public class DrawCircle       : IComputerVision { public int    cx         { get; set; } public int    cy         { get; set; } public int radius { get; set; } public int r { get; set; } public int g { get; set; } public int b { get; set; } public int thickness { get; set; } }
        public class DrawRectangle    : IComputerVision { public int    x          { get; set; } public int    y          { get; set; } public int width  { get; set; } public int height { get; set; } public int r { get; set; } public int g { get; set; } public int bl { get; set; } public int thickness { get; set; } }
        public class PutText          : IComputerVision { public string text       { get; set; } public int    x          { get; set; } public int y { get; set; } public double scale { get; set; } public int r { get; set; } public int g { get; set; } public int b { get; set; } }

        // Detection
        public class HoughLines       : IComputerVision { public double rho        { get; set; } public double theta      { get; set; } public int thresh { get; set; } }
        public class HoughCircles     : IComputerVision { public double dp         { get; set; } public double minDist    { get; set; } public double param1 { get; set; } public double param2 { get; set; } public int minRadius { get; set; } public int maxRadius { get; set; } }
        public class FindContours     : IComputerVision { public string mode       { get; set; } public string method     { get; set; } }

        // Arithmetic
        public class AddWeighted      : IComputerVision { public double alpha      { get; set; } public double beta       { get; set; } public double gamma { get; set; } }
        public class Normalize        : IComputerVision { public double alpha      { get; set; } public double beta       { get; set; } }
        public class BitwiseNot       : IComputerVision { }

        // I/O
        public class SaveFrame        : IComputerVision { public string path       { get; set; } }
        public class ShowWindow       : IComputerVision { public string title      { get; set; } }

        public class Factory
        {
            public static Dictionary<string, IComputerVision> features = new Dictionary<string, IComputerVision>()
            {
                {"Camera",          new Camera()         },
                {"ConvertColor",    new ConvertColor()   },
                {"Grayscale",       new Grayscale()      },
                {"Threshold",       new Threshold()      },
                {"AdaptiveThresh",  new AdaptiveThresh() },
                {"InRange",         new InRange()        },
                {"EqualizeHist",    new EqualizeHist()   },
                {"GaussianBlur",    new GaussianBlur()   },
                {"MedianBlur",      new MedianBlur()     },
                {"BilateralFilter", new BilateralFilter()},
                {"Blur",            new Blur()           },
                {"Erode",           new Erode()          },
                {"Dilate",          new Dilate()         },
                {"MorphOpen",       new MorphOpen()      },
                {"MorphClose",      new MorphClose()     },
                {"MorphGradient",   new MorphGradient()  },
                {"Canny",           new Canny()          },
                {"Sobel",           new Sobel()          },
                {"Laplacian",       new Laplacian()      },
                {"Resize",          new Resize()         },
                {"Rotate",          new Rotate()         },
                {"Flip",            new Flip()           },
                {"Crop",            new Crop()           },
                {"Warp",            new Warp()           },
                {"DrawCircle",      new DrawCircle()     },
                {"DrawRectangle",   new DrawRectangle()  },
                {"PutText",         new PutText()        },
                {"HoughLines",      new HoughLines()     },
                {"HoughCircles",    new HoughCircles()   },
                {"FindContours",    new FindContours()   },
                {"AddWeighted",     new AddWeighted()    },
                {"Normalize",       new Normalize()      },
                {"BitwiseNot",      new BitwiseNot()     },
                {"SaveFrame",       new SaveFrame()      },
                {"ShowWindow",      new ShowWindow()     },
            };

            public static IComputerVision FullFeature(string input)
            {
                string featureName = input.Split(':')[0].Replace("{", "").Replace("\n", "").Replace("\r", "").Replace(" ", "").Trim();
                if(featureName == "}" || featureName == "") return null;
                if(!features.ContainsKey(featureName)) return null;
                IComputerVision feature = features[featureName];
                string[] parameters = input[(input.IndexOf(':') + 1)..].Replace("\n", "").Replace("\r", "").Replace("{", "").Trim().Split(',');

                for(int i = 0; i < parameters.Length; i++){
                    parameters[i] = parameters[i].Split(':')[1].Trim();
                }

                switch(feature){
                    case Camera camera:{
                        ((Camera)feature).exp = int.Parse(parameters[0]);
                        break;
                    }
                    case ConvertColor convertColor:{
                        ((ConvertColor)feature).from = parameters[0];
                        ((ConvertColor)feature).to   = parameters[1];
                        break;
                    }
                    case Grayscale grayscale:{
                        break;
                    }
                    case Threshold threshold:{
                        ((Threshold)feature).thresh = double.Parse(parameters[0]);
                        ((Threshold)feature).maxval = double.Parse(parameters[1]);
                        ((Threshold)feature).type   = parameters[2];
                        break;
                    }
                    case AdaptiveThresh adaptiveThresh:{
                        ((AdaptiveThresh)feature).maxval    = double.Parse(parameters[0]);
                        ((AdaptiveThresh)feature).method    = parameters[1];
                        ((AdaptiveThresh)feature).type      = parameters[2];
                        ((AdaptiveThresh)feature).blockSize = int.Parse(parameters[3]);
                        ((AdaptiveThresh)feature).C         = double.Parse(parameters[4]);
                        break;
                    }
                    case InRange inRange:{
                        ((InRange)feature).lh = int.Parse(parameters[0]);
                        ((InRange)feature).ls = int.Parse(parameters[1]);
                        ((InRange)feature).lv = int.Parse(parameters[2]);
                        ((InRange)feature).uh = int.Parse(parameters[3]);
                        ((InRange)feature).us = int.Parse(parameters[4]);
                        ((InRange)feature).uv = int.Parse(parameters[5]);
                        break;
                    }
                    case EqualizeHist equalizeHist:{
                        break;
                    }
                    case GaussianBlur gaussianBlur:{
                        ((GaussianBlur)feature).ksize  = int.Parse(parameters[0]);
                        ((GaussianBlur)feature).sigmaX = double.Parse(parameters[1]);
                        break;
                    }
                    case MedianBlur medianBlur:{
                        ((MedianBlur)feature).ksize = int.Parse(parameters[0]);
                        break;
                    }
                    case BilateralFilter bilateralFilter:{
                        ((BilateralFilter)feature).d          = int.Parse(parameters[0]);
                        ((BilateralFilter)feature).sigmaColor = double.Parse(parameters[1]);
                        ((BilateralFilter)feature).sigmaSpace = double.Parse(parameters[2]);
                        break;
                    }
                    case Blur blur:{
                        ((Blur)feature).ksize = int.Parse(parameters[0]);
                        break;
                    }
                    case Erode erode:{
                        ((Erode)feature).ksize      = int.Parse(parameters[0]);
                        ((Erode)feature).iterations = int.Parse(parameters[1]);
                        break;
                    }
                    case Dilate dilate:{
                        ((Dilate)feature).ksize      = int.Parse(parameters[0]);
                        ((Dilate)feature).iterations = int.Parse(parameters[1]);
                        break;
                    }
                    case MorphOpen morphOpen:{
                        ((MorphOpen)feature).ksize = int.Parse(parameters[0]);
                        break;
                    }
                    case MorphClose morphClose:{
                        ((MorphClose)feature).ksize = int.Parse(parameters[0]);
                        break;
                    }
                    case MorphGradient morphGradient:{
                        ((MorphGradient)feature).ksize = int.Parse(parameters[0]);
                        break;
                    }
                    case Canny canny:{
                        ((Canny)feature).thresh1 = double.Parse(parameters[0]);
                        ((Canny)feature).thresh2 = double.Parse(parameters[1]);
                        break;
                    }
                    case Sobel sobel:{
                        ((Sobel)feature).dx    = int.Parse(parameters[0]);
                        ((Sobel)feature).dy    = int.Parse(parameters[1]);
                        ((Sobel)feature).ksize = int.Parse(parameters[2]);
                        break;
                    }
                    case Laplacian laplacian:{
                        ((Laplacian)feature).ksize = int.Parse(parameters[0]);
                        break;
                    }
                    case Resize resize:{
                        ((Resize)feature).width  = int.Parse(parameters[0]);
                        ((Resize)feature).height = int.Parse(parameters[1]);
                        break;
                    }
                    case Rotate rotate:{
                        ((Rotate)feature).angle = double.Parse(parameters[0]);
                        break;
                    }
                    case Flip flip:{
                        ((Flip)feature).flipCode = int.Parse(parameters[0]);
                        break;
                    }
                    case Crop crop:{
                        ((Crop)feature).x      = int.Parse(parameters[0]);
                        ((Crop)feature).y      = int.Parse(parameters[1]);
                        ((Crop)feature).width  = int.Parse(parameters[2]);
                        ((Crop)feature).height = int.Parse(parameters[3]);
                        break;
                    }
                    case Warp warp:{
                        ((Warp)feature).m00 = double.Parse(parameters[0]);
                        ((Warp)feature).m01 = double.Parse(parameters[1]);
                        ((Warp)feature).m02 = double.Parse(parameters[2]);
                        ((Warp)feature).m10 = double.Parse(parameters[3]);
                        ((Warp)feature).m11 = double.Parse(parameters[4]);
                        ((Warp)feature).m12 = double.Parse(parameters[5]);
                        break;
                    }
                    case DrawCircle drawCircle:{
                        ((DrawCircle)feature).cx        = int.Parse(parameters[0]);
                        ((DrawCircle)feature).cy        = int.Parse(parameters[1]);
                        ((DrawCircle)feature).radius    = int.Parse(parameters[2]);
                        ((DrawCircle)feature).r         = int.Parse(parameters[3]);
                        ((DrawCircle)feature).g         = int.Parse(parameters[4]);
                        ((DrawCircle)feature).b         = int.Parse(parameters[5]);
                        ((DrawCircle)feature).thickness = int.Parse(parameters[6]);
                        break;
                    }
                    case DrawRectangle drawRectangle:{
                        ((DrawRectangle)feature).x         = int.Parse(parameters[0]);
                        ((DrawRectangle)feature).y         = int.Parse(parameters[1]);
                        ((DrawRectangle)feature).width     = int.Parse(parameters[2]);
                        ((DrawRectangle)feature).height    = int.Parse(parameters[3]);
                        ((DrawRectangle)feature).r         = int.Parse(parameters[4]);
                        ((DrawRectangle)feature).g         = int.Parse(parameters[5]);
                        ((DrawRectangle)feature).bl        = int.Parse(parameters[6]);
                        ((DrawRectangle)feature).thickness = int.Parse(parameters[7]);
                        break;
                    }
                    case PutText putText:{
                        ((PutText)feature).text  = parameters[0];
                        ((PutText)feature).x     = int.Parse(parameters[1]);
                        ((PutText)feature).y     = int.Parse(parameters[2]);
                        ((PutText)feature).scale = double.Parse(parameters[3]);
                        ((PutText)feature).r     = int.Parse(parameters[4]);
                        ((PutText)feature).g     = int.Parse(parameters[5]);
                        ((PutText)feature).b     = int.Parse(parameters[6]);
                        break;
                    }
                    case HoughLines houghLines:{
                        ((HoughLines)feature).rho    = double.Parse(parameters[0]);
                        ((HoughLines)feature).theta  = double.Parse(parameters[1]);
                        ((HoughLines)feature).thresh = int.Parse(parameters[2]);
                        break;
                    }
                    case HoughCircles houghCircles:{
                        ((HoughCircles)feature).dp        = double.Parse(parameters[0]);
                        ((HoughCircles)feature).minDist   = double.Parse(parameters[1]);
                        ((HoughCircles)feature).param1    = double.Parse(parameters[2]);
                        ((HoughCircles)feature).param2    = double.Parse(parameters[3]);
                        ((HoughCircles)feature).minRadius = int.Parse(parameters[4]);
                        ((HoughCircles)feature).maxRadius = int.Parse(parameters[5]);
                        break;
                    }
                    case FindContours findContours:{
                        ((FindContours)feature).mode   = parameters[0];
                        ((FindContours)feature).method = parameters[1];
                        break;
                    }
                    case AddWeighted addWeighted:{
                        ((AddWeighted)feature).alpha = double.Parse(parameters[0]);
                        ((AddWeighted)feature).beta  = double.Parse(parameters[1]);
                        ((AddWeighted)feature).gamma = double.Parse(parameters[2]);
                        break;
                    }
                    case Normalize normalize:{
                        ((Normalize)feature).alpha = double.Parse(parameters[0]);
                        ((Normalize)feature).beta  = double.Parse(parameters[1]);
                        break;
                    }
                    case BitwiseNot bitwiseNot:{
                        break;
                    }
                    case SaveFrame saveFrame:{
                        ((SaveFrame)feature).path = parameters[0];
                        break;
                    }
                    case ShowWindow showWindow:{
                        ((ShowWindow)feature).title = parameters[0];
                        break;
                    }
                    case null:{
                        break;
                    }
                }
                return feature;
            }
        }

        public static Mat RunPipeline(IComputerVision computerVision, VideoCapture capture, Mat frame)
        {
            switch(computerVision){
                case Camera camera:{
                    Console.WriteLine($"Camera with exp {camera.exp}");
                    capture.Set(VideoCaptureProperties.Exposure, camera.exp);
                    capture.Read(frame);
                    break;
                }
                case ConvertColor convertColor:{
                    Console.WriteLine($"Convert from {convertColor.from} to {convertColor.to}");
                    Cv2.CvtColor(frame, frame, ColorConversionCodes.BGR2GRAY);
                    break;
                }
                case Grayscale grayscale:{
                    Console.WriteLine($"Grayscale");
                    Cv2.CvtColor(frame, frame, ColorConversionCodes.BGR2GRAY);
                    break;
                }
                case Threshold threshold:{
                    Console.WriteLine($"Threshold thresh={threshold.thresh} maxval={threshold.maxval} type={threshold.type}");
                    ThresholdTypes tt = threshold.type.Trim() switch {
                        "BINARY_INV" => ThresholdTypes.BinaryInv,
                        "TRUNC"      => ThresholdTypes.Trunc,
                        "TOZERO"     => ThresholdTypes.Tozero,
                        "TOZERO_INV" => ThresholdTypes.TozeroInv,
                        "OTSU"       => ThresholdTypes.Otsu,
                        _            => ThresholdTypes.Binary
                    };
                    Cv2.Threshold(frame, frame, threshold.thresh, threshold.maxval, tt);
                    break;
                }
                case AdaptiveThresh adaptiveThresh:{
                    Console.WriteLine($"AdaptiveThresh method={adaptiveThresh.method} blockSize={adaptiveThresh.blockSize}");
                    var method = adaptiveThresh.method.Trim() == "GAUSSIAN"
                        ? AdaptiveThresholdTypes.GaussianC
                        : AdaptiveThresholdTypes.MeanC;
                    var type = adaptiveThresh.type.Trim() == "BINARY_INV"
                        ? ThresholdTypes.BinaryInv
                        : ThresholdTypes.Binary;
                    Cv2.AdaptiveThreshold(frame, frame, adaptiveThresh.maxval, method, type, adaptiveThresh.blockSize, adaptiveThresh.C);
                    break;
                }
                case InRange inRange:{
                    Console.WriteLine($"InRange lower=({inRange.lh},{inRange.ls},{inRange.lv}) upper=({inRange.uh},{inRange.us},{inRange.uv})");
                    Cv2.InRange(frame, new Scalar(inRange.lh, inRange.ls, inRange.lv), new Scalar(inRange.uh, inRange.us, inRange.uv), frame);
                    break;
                }
                case EqualizeHist equalizeHist:{
                    Console.WriteLine($"EqualizeHist");
                    Cv2.EqualizeHist(frame, frame);
                    break;
                }
                case GaussianBlur gaussianBlur:{
                    int k = gaussianBlur.ksize % 2 == 0 ? gaussianBlur.ksize + 1 : gaussianBlur.ksize;
                    Console.WriteLine($"GaussianBlur ksize={k} sigmaX={gaussianBlur.sigmaX}");
                    Cv2.GaussianBlur(frame, frame, new Size(k, k), gaussianBlur.sigmaX);
                    break;
                }
                case MedianBlur medianBlur:{
                    int k = medianBlur.ksize % 2 == 0 ? medianBlur.ksize + 1 : medianBlur.ksize;
                    Console.WriteLine($"MedianBlur ksize={k}");
                    Cv2.MedianBlur(frame, frame, k);
                    break;
                }
                case BilateralFilter bilateralFilter:{
                    Console.WriteLine($"BilateralFilter d={bilateralFilter.d}");
                    using var tmp = new Mat();
                    Cv2.BilateralFilter(frame, tmp, bilateralFilter.d, bilateralFilter.sigmaColor, bilateralFilter.sigmaSpace);
                    tmp.CopyTo(frame);
                    break;
                }
                case Blur blur:{
                    Console.WriteLine($"Blur ksize={blur.ksize}");
                    Cv2.Blur(frame, frame, new Size(blur.ksize, blur.ksize));
                    break;
                }
                case Erode erode:{
                    Console.WriteLine($"Erode ksize={erode.ksize} iterations={erode.iterations}");
                    var kernel = Cv2.GetStructuringElement(MorphShapes.Rect, new Size(erode.ksize, erode.ksize));
                    Cv2.Erode(frame, frame, kernel, iterations: erode.iterations);
                    break;
                }
                case Dilate dilate:{
                    Console.WriteLine($"Dilate ksize={dilate.ksize} iterations={dilate.iterations}");
                    var kernel = Cv2.GetStructuringElement(MorphShapes.Rect, new Size(dilate.ksize, dilate.ksize));
                    Cv2.Dilate(frame, frame, kernel, iterations: dilate.iterations);
                    break;
                }
                case MorphOpen morphOpen:{
                    Console.WriteLine($"MorphOpen ksize={morphOpen.ksize}");
                    var kernel = Cv2.GetStructuringElement(MorphShapes.Rect, new Size(morphOpen.ksize, morphOpen.ksize));
                    Cv2.MorphologyEx(frame, frame, MorphTypes.Open, kernel);
                    break;
                }
                case MorphClose morphClose:{
                    Console.WriteLine($"MorphClose ksize={morphClose.ksize}");
                    var kernel = Cv2.GetStructuringElement(MorphShapes.Rect, new Size(morphClose.ksize, morphClose.ksize));
                    Cv2.MorphologyEx(frame, frame, MorphTypes.Close, kernel);
                    break;
                }
                case MorphGradient morphGradient:{
                    Console.WriteLine($"MorphGradient ksize={morphGradient.ksize}");
                    var kernel = Cv2.GetStructuringElement(MorphShapes.Rect, new Size(morphGradient.ksize, morphGradient.ksize));
                    Cv2.MorphologyEx(frame, frame, MorphTypes.Gradient, kernel);
                    break;
                }
                case Canny canny:{
                    Console.WriteLine($"Canny thresh1={canny.thresh1} thresh2={canny.thresh2}");
                    Cv2.Canny(frame, frame, canny.thresh1, canny.thresh2);
                    break;
                }
                case Sobel sobel:{
                    Console.WriteLine($"Sobel dx={sobel.dx} dy={sobel.dy} ksize={sobel.ksize}");
                    Cv2.Sobel(frame, frame, MatType.CV_8U, sobel.dx, sobel.dy, sobel.ksize);
                    break;
                }
                case Laplacian laplacian:{
                    Console.WriteLine($"Laplacian ksize={laplacian.ksize}");
                    Cv2.Laplacian(frame, frame, MatType.CV_8U, laplacian.ksize);
                    break;
                }
                case Resize resize:{
                    Console.WriteLine($"Resize {resize.width}x{resize.height}");
                    Cv2.Resize(frame, frame, new Size(resize.width, resize.height));
                    break;
                }
                case Rotate rotate:{
                    Console.WriteLine($"Rotate angle={rotate.angle}");
                    var center = new Point2f(frame.Width / 2f, frame.Height / 2f);
                    var M = Cv2.GetRotationMatrix2D(center, rotate.angle, 1.0);
                    Cv2.WarpAffine(frame, frame, M, frame.Size());
                    break;
                }
                case Flip flip:{
                    Console.WriteLine($"Flip flipCode={flip.flipCode}");
                    Cv2.Flip(frame, frame, (FlipMode)flip.flipCode);
                    break;
                }
                case Crop crop:{
                    Console.WriteLine($"Crop ({crop.x},{crop.y}) {crop.width}x{crop.height}");
                    frame = new Mat(frame, new Rect(crop.x, crop.y, crop.width, crop.height)).Clone();
                    break;
                }
                case Warp warp:{
                    Console.WriteLine($"Warp affine");
                    double[,] data = { { warp.m00, warp.m01, warp.m02 }, { warp.m10, warp.m11, warp.m12 } };
                    var M = Mat.FromArray(data);
                    Cv2.WarpAffine(frame, frame, M, frame.Size());
                    break;
                }
                case DrawCircle drawCircle:{
                    Console.WriteLine($"DrawCircle center=({drawCircle.cx},{drawCircle.cy}) radius={drawCircle.radius}");
                    Cv2.Circle(frame, new Point(drawCircle.cx, drawCircle.cy), drawCircle.radius, new Scalar(drawCircle.b, drawCircle.g, drawCircle.r), drawCircle.thickness);
                    break;
                }
                case DrawRectangle drawRectangle:{
                    Console.WriteLine($"DrawRectangle ({drawRectangle.x},{drawRectangle.y}) {drawRectangle.width}x{drawRectangle.height}");
                    Cv2.Rectangle(frame, new Rect(drawRectangle.x, drawRectangle.y, drawRectangle.width, drawRectangle.height), new Scalar(drawRectangle.bl, drawRectangle.g, drawRectangle.r), drawRectangle.thickness);
                    break;
                }
                case PutText putText:{
                    Console.WriteLine($"PutText \"{putText.text}\" at ({putText.x},{putText.y})");
                    Cv2.PutText(frame, putText.text, new Point(putText.x, putText.y), HersheyFonts.HersheySimplex, putText.scale, new Scalar(putText.b, putText.g, putText.r));
                    break;
                }
                case HoughLines houghLines:{
                    Console.WriteLine($"HoughLines rho={houghLines.rho} theta={houghLines.theta} thresh={houghLines.thresh}");
                    var lines = Cv2.HoughLines(frame, houghLines.rho, houghLines.theta, houghLines.thresh);
                    foreach(var line in lines){
                        double a = Math.Cos(line.Rho), b = Math.Sin(line.Rho);
                        double x0 = a * line.Rho, y0 = b * line.Rho;
                        var pt1 = new Point((int)(x0 + 1000 * (-b)), (int)(y0 + 1000 * a));
                        var pt2 = new Point((int)(x0 - 1000 * (-b)), (int)(y0 - 1000 * a));
                        Cv2.Line(frame, pt1, pt2, Scalar.Red, 2);
                    }
                    break;
                }
                case HoughCircles houghCircles:{
                    Console.WriteLine($"HoughCircles dp={houghCircles.dp}");
                    var circles = Cv2.HoughCircles(frame, HoughModes.Gradient, houghCircles.dp, houghCircles.minDist, houghCircles.param1, houghCircles.param2, houghCircles.minRadius, houghCircles.maxRadius);
                    foreach(var c in circles){
                        Cv2.Circle(frame, (int)c.Center.X, (int)c.Center.Y, (int)c.Radius, Scalar.Red, 2);
                        Cv2.Circle(frame, (int)c.Center.X, (int)c.Center.Y, 3, Scalar.Green, -1);
                    }
                    break;
                }
                case FindContours findContours:{
                    Console.WriteLine($"FindContours mode={findContours.mode} method={findContours.method}");
                    var mode   = findContours.mode.Trim()   == "EXTERNAL" ? RetrievalModes.External : RetrievalModes.List;
                    var method = findContours.method.Trim() == "SIMPLE"   ? ContourApproximationModes.ApproxSimple : ContourApproximationModes.ApproxNone;
                    Cv2.FindContours(frame, out var contours, out _, mode, method);
                    using var coloured = new Mat();
                    if(frame.Channels() == 1) Cv2.CvtColor(frame, coloured, ColorConversionCodes.GRAY2BGR);
                    else frame.CopyTo(coloured);
                    Cv2.DrawContours(coloured, contours, -1, Scalar.Green, 2);
                    coloured.CopyTo(frame);
                    break;
                }
                case AddWeighted addWeighted:{
                    Console.WriteLine($"AddWeighted alpha={addWeighted.alpha} beta={addWeighted.beta} gamma={addWeighted.gamma}");
                    Cv2.AddWeighted(frame, addWeighted.alpha, frame, addWeighted.beta, addWeighted.gamma, frame);
                    break;
                }
                case Normalize normalize:{
                    Console.WriteLine($"Normalize alpha={normalize.alpha} beta={normalize.beta}");
                    Cv2.Normalize(frame, frame, normalize.alpha, normalize.beta, NormTypes.MinMax);
                    break;
                }
                case BitwiseNot bitwiseNot:{
                    Console.WriteLine($"BitwiseNot");
                    Cv2.BitwiseNot(frame, frame);
                    break;
                }
                case SaveFrame saveFrame:{
                    Console.WriteLine($"SaveFrame path={saveFrame.path}");
                    Cv2.ImWrite(saveFrame.path.Trim(), frame);
                    break;
                }
                case ShowWindow showWindow:{
                    Console.WriteLine($"ShowWindow title={showWindow.title}");
                    Cv2.ImShow(showWindow.title.Trim(), frame);
                    break;
                }
            }
            return frame;
        }

        public static Mat MacCameraFeed(string file)
        {
            List<IComputerVision> pipeline = new List<IComputerVision>();
            file = file.Replace("\"", "");
            foreach(string line in file.Split("},")){
                pipeline.Add(Factory.FullFeature(line));
            }
            using (var capture = new VideoCapture(0)){
                    if (!capture.IsOpened()){
                        Console.WriteLine("Error: Camera could not be opened.");
                        return null;
                    }

                    using (var frame = new Mat()){
                        while (true){
                            capture.Read(frame);

                            foreach(IComputerVision feature in pipeline){
                                if(feature == null) continue;
                                return RunPipeline(feature, capture, frame);
                            }

                            if (frame.Empty()) break;
                            Cv2.ImShow("Mac Camera Feed", frame);
                            if(Cv2.WaitKey(1) == 27) break;
                        }
                    }
                }
                return null;
        }

        static void Main(string[] args)
        {
            while(true){}
            // string file = File.ReadAllText("../../../../JSONFile.txt");
            // 

            // MacCameraFeed(pipeline);
        }
    }
}
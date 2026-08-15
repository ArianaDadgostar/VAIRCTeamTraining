
using OpenCvSharp;

// var builder = WebApplication.CreateBuilder(args);

// // Add services to the container.
// // Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
// builder.Services.AddEndpointsApiExplorer();
// builder.Services.AddSwaggerGen();

// var app = builder.Build();

// // Configure the HTTP request pipeline.
// if (app.Environment.IsDevelopment())
// {
//     app.UseSwagger();
//     app.UseSwaggerUI();
// }

// app.UseHttpsRedirection();

// var summaries = new[]
// {
//     "Freezing", "Bracing", "Chilly", "Cool", "Mild", "Warm", "Balmy", "Hot", "Sweltering", "Scorching"
// };

// app.MapGet("/weatherforecast", () =>
// {
//     var forecast =  Enumerable.Range(1, 5).Select(index =>
//         new WeatherForecast
//         (
//             DateOnly.FromDateTime(DateTime.Now.AddDays(index)),
//             Random.Shared.Next(-20, 55),
//             summaries[Random.Shared.Next(summaries.Length)]
//         ))
//         .ToArray();
//     return forecast;
// })
// .WithName("GetWeatherForecast")
// .WithOpenApi();

//app.Run();

record WeatherForecast(DateOnly Date, int TemperatureC, string? Summary)
{
    public int TemperatureF => 32 + (int)(TemperatureC / 0.5556);
}

namespace DemosaicAPI
{
    static class PixelGrouping
    {
        static int nChange;
        static int eChange;
        static int wChange;
        static int sChange;
        static int Hue_Transit(int g1, int g2, int g3, int v1, int v3)
        {
            if((g1 < g2 && g2 < g3) || (g1 > g2 && g2 > g3)) 
                return v1 + ((v3 - v1) * (g2 - g1) / (g3 - g1));
            
            return (v1 + v3) / 2 + (g2 * 2 - g1 - g3) / 4;
        }

        static bool IsSmallest(int chosen, int val1, int val2, int val3)
        {
            if(chosen > val1 || chosen > val2 || chosen > val3) return false;

            return true;
        }

        static int Calculate_Green(int north, int east, int west, int south, int current,
                                   int gNorth, int gEast, int gWest, int gSouth)
        {
            nChange = Math.Abs(north - current) * 2 + Math.Abs(gNorth - gSouth);
            eChange = Math.Abs(east - current) * 2 + Math.Abs(gEast - gWest);
            wChange = Math.Abs(west - current) * 2 + Math.Abs(gEast - gWest);
            sChange = Math.Abs(south - current) * 2 + Math.Abs(gNorth - gSouth);

            if(IsSmallest(nChange, eChange, wChange, sChange)) 
                return (gNorth * 3 + gSouth + current - north) / 4;
            if(IsSmallest(eChange, nChange, wChange, sChange))
                return (gEast * 3 + gWest + current - east) / 4;
            if(IsSmallest(wChange, nChange, eChange, sChange))
                return (gWest * 3 + gEast + current - west) / 4;
            else 
                return (gSouth * 3 + gNorth + current - south) / 4;
        }

        static int Calculate_Diagonal(int current, int NE, int SE, int NW, int SW, 
                                      int altNE, int altSE, int altNW, int altSW,
                                      int gCurrent, int gNE, int gSE, int gNW, int gSW)
        {
            eChange = Math.Abs(altNE - altSW) + Math.Abs(NE - current) 
                    + Math.Abs(current - SW) + Math.Abs(gNE - gCurrent)
                    + Math.Abs(gCurrent - gSW);
            wChange = Math.Abs(altNW - altSE) + Math.Abs(NW - current) 
                    + Math.Abs(current - SE) + Math.Abs(gNW - gCurrent)
                    + Math.Abs(gCurrent - gSE);
            
            if(IsSmallest(eChange, wChange, eChange + 1, eChange + 1))
                return Hue_Transit(gNE, gCurrent, gSW, altNE, altSW);
            else
                return Hue_Transit(gNW, gCurrent, gSE, altNW, altSE);
        }
    }

    class Program
    {
        static Mat mat = new Mat("C:\\Users\\User\\Pictures\\Camera Roll\\IMG_20230610_123456.jpg", ImreadModes.Color);
        static void Main(string[] args)
        {
            for(int i = 0; i < mat.Rows; i++)
            {
                for(int j = 0; j < mat.Cols; j += 2)
                {
                    Vec3b pixel = mat.At<Vec3b>(i, j);
                    Console.WriteLine($"Pixel at ({i}, {j}): B={pixel.Item0}, G={pixel.Item1}, R={pixel.Item2}");
                    break;
                }
                break;
            }
        }
    }
}
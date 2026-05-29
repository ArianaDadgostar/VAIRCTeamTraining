// See https://aka.ms/new-console-template for more information

using System.ComponentModel.Design.Serialization;

using System;
using System.Security.Cryptography;
using Microsoft.VisualBasic;
using System.Drawing;
using System.Reflection.Metadata.Ecma335;
using System.Runtime.Versioning;

class Program
{
    public class IComputerVision {}
    class Camera : IComputerVision { public int exp {get; set; } }
    class ConvertColor : IComputerVision { public string to {get; set; } public string from {get; set; } }

    class Factory
    {
        public static Dictionary<string, IComputerVision> features = new Dictionary<string, IComputerVision>()
        {
            {"Camera", new Camera()},
            {"ConvertColor", new ConvertColor()}
        };

        public static IComputerVision FullFeature(string input)
        {
            string featureName = input.Split(':')[0].Replace("{\n", "").Trim();
            if(featureName == "}") return null;
            IComputerVision feature = features[featureName];
            string[] parameters = input[(input.IndexOf(':') + 1)..].Replace("\n", "").Replace("{", "").Trim().Split(',');

            for(int i = 0; i < parameters.Length; i++){
                parameters[i] = parameters[i].Split(':')[1];
            }

            switch(feature){
                case Camera camera:{
                    ((Camera)feature).exp = int.Parse(parameters[0]);
                    break;
                }
                case ConvertColor convertColor:{
                    ((ConvertColor)feature).from = parameters[0];
                    ((ConvertColor)feature).to = parameters[1];
                    break;
                }
                case null:{
                    break;
                }
            }
            return feature;
        }
    }

    public void RunPipeline(IComputerVision computerVision)
    {
        switch(computerVision){
            case Camera camera:{
                Console.WriteLine($"Camera with exp {camera.exp}");
                break;
            }
            case ConvertColor convertColor:{
                Console.WriteLine($"Convert from {convertColor.from} to {convertColor.to}");
                break;
            }
        }
    }

    static void Main(string[] args)
    {
        string file = File.ReadAllText("../../../JSONFile.txt");
        List<IComputerVision> pipeline = new List<IComputerVision>();
        file = file.Replace("\"", "");
        foreach(string line in file.Split("},")){
            pipeline.Add(Factory.FullFeature(line));
        }
        foreach(IComputerVision feature in pipeline){
            if(feature == null) continue;
            new Program().RunPipeline(feature);
        }
    }
}

/* 
Create independent classes per feature

*/
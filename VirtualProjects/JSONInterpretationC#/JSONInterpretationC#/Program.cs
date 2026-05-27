// See https://aka.ms/new-console-template for more information

using System.ComponentModel.Design.Serialization;

using System;
using System.Security.Cryptography;
using Microsoft.VisualBasic;
using System.Drawing;

class Program
{
    public interface IComputerVision { public void Init(string input);}
    class Camera : IComputerVision
    {
        int exposure;
        public void Init(string input)
        {
            exposure = int.Parse(input);
        }
    }

    class ConvertColor : IComputerVision
    {
        string from;
        string to;
        public void Init(string input)
        {
            string parameter = "";
            string value = "";
            foreach(char c in input)
            {
                if(c == '"' || c == ':') continue;
                if(c == ',')
                {
                     
                }
                parameter = () ? 
            }
        }
    }

    Dictionary<string, IComputerVision> functions = new Dictionary<string, IComputerVision>()
    {
        {"Camera", new Camera()},
        {"ConvertColor", new ConvertColor()}
    };
    static void InterpretFile(string file)
    {
        string function = "";
        List<string> parameters = new List<string>();
        bool isFunction = false;
        foreach(char current in file)
        {
            isFunction = (current == '"' && function.Length == 0);
            function = (isFunction) ? function + current : function;
        }
    }

    static void Main(string[] args)
    {
        string file = "";
    }
}

/* 
Create independent classes per feature

*/
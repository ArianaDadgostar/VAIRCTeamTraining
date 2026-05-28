// See https://aka.ms/new-console-template for more information

using System.ComponentModel.Design.Serialization;

using System;
using System.Security.Cryptography;
using Microsoft.VisualBasic;
using System.Drawing;
using System.Reflection.Metadata.Ecma335;

class Program
{
    public interface IComputerVision {}
    class Camera : IComputerVision { int exp {get; set; } }
    class ConvertColor : IComputerVision { string to {get; set; } string from {get; set; } }

    public void Interpret(string input, IComputerVision computerVision)
    {
        string parameter = "";
        string value = "";
        foreach(char c in input)
        {
            if(c == '"') continue;
            else if(c == ':')
            {
                value = "_s";
            }
            
            if(c == ',')
            {
                if(int.TryParse(value, out int output))
                {
                    computerVision.parameters.Add(parameter, output);
                }
                else 
                {
                    computerVision.parameters.Add(parameter, value);
                }

                parameter = "";
                value = "";
                continue;
            }
            parameter = (value == "") ? parameter + c : parameter;
            value = (value == "_s") ? c.ToString() : value + c;
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
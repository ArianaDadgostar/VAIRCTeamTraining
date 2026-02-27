// See https://aka.ms/new-console-template for more information

using System.ComponentModel.Design.Serialization;

using System;

public class Analysis
{
    public Dictionary<string, List<string>> storage;
    public int maxWordCount;
    public int currentWordCount;
    public string last;
    public string generated;

    public Analysis(string File)
    {
        string previous = "";
        string current = "";

        foreach(char var in File)
        {
            if(var != ' ')
            {
                current += var;
                continue;
            }

            if(previous == "")
            {
                previous = current;
                current = "";

                storage.Add(previous, new List<string>());

                continue;
            }

            storage[previous].Add(current);

            previous = current;
            current = "";

            if(storage.ContainsKey(previous)) continue;


            storage.Add(previous, new List<string>());
        }
    }

    public void Establish(string File)
    {
        string previous = "";
        string current = "";

        foreach(char var in File)
        {
            if(var != ' ')
            {
                current += var;
                continue;
            }

            if(previous == "")
            {
                previous = current;
                current = "";

                storage.Add(previous, new List<string>());

                continue;
            }

            storage[previous].Add(current);

            previous = current;
            current = "";

            if(storage.ContainsKey(previous)) continue;


            storage.Add(previous, new List<string>());
        }
    }

    public bool CanGenerate()
    {
        if(last == "" || storage[last].Count == 0) return false;

        generated += " ";

        Random random = new Random();

        string current = storage[last][random.Next()%(storage[last].Count)];

        generated += current;

        last = current; 
        currentWordCount++;

        return true;
    }
}

class Program
{
    static void Main(string[] args)
    {
        string file = "Trees are important for life on Earth. Trees provide oxygen for people and animals. Trees help clean the air and help the planet stay healthy. Trees grow in forests and parks and near rivers. Trees have roots that hold the soil in place, and trees have leaves that take in sunlight. Trees give homes to birds and insects, and trees give shade to people. Trees help reduce pollution, and trees help fight climate change. Trees are important for nature, and trees are important for life on Earth.";

        Analysis analysis = new Analysis();
        analysis.maxWordCount = 40;

        analysis.Establish(file);

        analysis.last = "Trees";
        analysis.generated += analysis.last;

        while(analysis.CanGenerate() && analysis.currentWordCount <= analysis.maxWordCount){}

        Console.WriteLine(analysis.generated);

    }
}
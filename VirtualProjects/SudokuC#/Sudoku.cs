using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Security.Principal;

namespace SudokuC_;


public class Sudoku
{
    public LinkedList<int[]> possibilities;

    public List<int[]> setup;

    public int[] allowedNumbers;

    public Sudoku(List<int[]> setup, int[] allowedNumbers)
    {
        this.setup = setup;
        this.allowedNumbers = allowedNumbers;
        this.possibilities = new LinkedList<int[]>();
    }

    public void GenerateNumber(List<int> indexes, List<int> remaining, int index, List<int[]> gathered, int[] number)
    {
        if(indexes.Count == 0) return;
        Console.WriteLine("index count" + indexes.Count);
        int current = indexes[0];
        indexes.Remove(current);

        int chosen;
        for(int i = 0; i < remaining.Count; i++)
        {
            if(remaining[i] == 0) continue;

            chosen = remaining[i];
            remaining[i] = 0;

            number[current] = chosen;

            if(indexes.Count == 0) gathered.Add(number);

            GenerateNumber(indexes, remaining, index + 1, gathered, number);

            remaining[i] = chosen;
        }
    }

    public void SetupPossibilities(List<int> remaining, List<int> indexes)
    {
        foreach(int[] row in setup)
        {
            remaining.AddRange(allowedNumbers);
            for(int i = 0; i < allowedNumbers.Length; i++)
            {
                remaining.Remove(row[i]);
                if(row[i] != 0) continue;

                indexes.Add(i);
            }

            List<int[]> generated = new();
            GenerateNumber(indexes, remaining, 0, generated, new int[allowedNumbers.Length]);
            int[] previous;
            for(int i = 0; i < generated.Count; i++)
            {
                previous = i == 0 ? null : generated[i - 1];
                possibilities.Insert(generated[i], previous);
            }
        }
    }

    public void EliminateImpossible(List<int> taken, List<int> takenIndexes)
    {
        Node<int[]> current = possibilities.head;
        for(int i = 0; i < possibilities.GetCount(); i++)
        {
            for(int j = allowedNumbers.Length - 1; j >= 0; j--)
            {
                //if()
            }
            current = current.next;
        }
    }

    public void TraversePossibilities()
    {
        
    }
}
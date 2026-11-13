using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Security.Principal;

namespace SudokuC_;

public class Sudoku
{
    public LinkedList<int> possibilities;

    public List<int[]> setup;

    public int[] allowedNumbers;

    public Sudoku(List<int[]> setup, int[] allowedNumbers)
    {
        this.setup = setup;
        this.allowedNumbers = allowedNumbers;
    }

    public void GenerateNumber(List<int> indexes, List<int> remaining, int row, int index, List<int> gathered, int number)
    {
        if(indexes.Count == 0) return;

        int current = indexes[index];
        indexes.Remove(current);

        int chosen;
        for(int i = 0; i < remaining.Count; i++)
        {
            if(remaining[i] == 0) continue;

            chosen = remaining[i];
            remaining[i] = 0;

            number += chosen * (int)Math.Pow(10, current);

            if(indexes.Count == 0) gathered.Add(number);

            GenerateNumber(indexes, remaining, row, index + 1, gathered, number);

            remaining[i] = chosen;
        }
    }

    public void SetupPossibilities(List<int> remaining, List<int> indexes)
    {
        int next;
        foreach(int row in setup)
        {
            next = row;
            for(int i = allowedNumbers.Length - 1; i >= 0; i--)
            {
                int value = (int) Math.Floor(next / Math.Pow(10, i));

                next -= value * (int)Math.Pow(10, i);

                if(value != 0) continue;

                remaining.Add(value);
                indexes.Add(i);
            }

            List<int> generated = new();
            GenerateNumber(indexes, remaining, row, 0, generated, 0);
            int previous;
            for(int i = 0; i < generated.Count; i++)
            {
                previous = i == 0 ? 0 : generated[i - 1];
                possibilities.Insert(generated[i], previous);
            }
        }
    }

    public void EliminateImpossible(List<int> taken, List<int> takenIndexes)
    {
        int next;
        Node<int> current = possibilities.head;
        for(int i = 0; i < possibilities.GetCount(); i++)
        {
            next = current.value;
            for(int j = allowedNumbers.Length - 1; j >= 0; j--)
            {
                int value = (int) Math.Floor(next / Math.Pow(10, i));

                next -= value * (int)Math.Pow(10, i);
                
                if()
            }
            current = current.next;
        }
    }

    public void TraversePossibilities()
    {
        
    }
}
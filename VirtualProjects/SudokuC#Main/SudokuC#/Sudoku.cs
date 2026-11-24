using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
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

    public void GenerateNumber(List<int> indexes, List<int> remaining, List<int[]> gathered, int[] number, int currentIndex)
    {
        if(indexes.Count == 0) return;

        int chosen;
        int current;
        for(int i = 0; i < remaining.Count; i++)
        {
            if(remaining[i] == 0) continue;
            
            current = indexes[currentIndex];
            chosen = remaining[i];
            indexes[currentIndex] = 0;
            remaining[i] = 0;

            number[current] = chosen;

            if(indexes.Max() == 0) // CHECKS IF ALL INDEXES ARE SATISFIED
            {
                gathered.Add(number);
            }
            else GenerateNumber(indexes, remaining, gathered, number, currentIndex + 1);

            remaining[i] = chosen;
            indexes[currentIndex] = current;
        }
    }

    public void DefinePossibilities(List<int[]> generated, int[] row)
    {
        for(int i = 0; i < generated.Count; i++)
        {
            for(int j = 0; j < generated[i].Length; j++)
            {
                if(generated[i][j] != 0) continue;
                
                generated[i][j] = row[j];
            }
            
            int[] previous = i == 0 ? null : generated[i - 1];
            possibilities.Insert(generated[i], previous);
        }
    }

    public void SetupPossibilities(List<int> remaining, List<int> indexes)
    {
        foreach(int[] row in setup)
        {
            remaining.Clear();
            indexes.Clear();
            remaining.AddRange(allowedNumbers);
            for(int i = 0; i < allowedNumbers.Length; i++)
            {
                if(remaining.Remove(row[i])) continue;

                indexes.Add(i);
            }

            List<int[]> generated = new();
            GenerateNumber(indexes, remaining, generated, new int[allowedNumbers.Length], 0);
            DefinePossibilities(generated, row);
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
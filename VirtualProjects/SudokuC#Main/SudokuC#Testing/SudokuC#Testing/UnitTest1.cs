using System.Reflection.Metadata;
using SudokuC_;

namespace SudokuC_Testing;

public class UnitTest1
{
    [Theory]
    [InlineData(new int[] { 1, 2, 3, 4, 5, 6, 7, 8, 9 })]
    public void InsertTest(params int[] numbers)
    {
        SudokuC_.LinkedList<int> SudokuC = new();
        int previous = 0;
        foreach(var number in numbers)
        {
            Assert.True(SudokuC.Insert(number, previous));
            Assert.True(SudokuC.tail.next.value.Equals(SudokuC.head.value));
            Console.WriteLine(SudokuC.head.previous.value);
            Console.WriteLine(SudokuC.tail.value);
            Assert.True(SudokuC.head.previous.value.Equals(SudokuC.tail.value));
            
            previous = number;
        }

        Node<int> current = SudokuC.head;
        for(int i = 0; i < numbers.Length; i++)
        {
            Assert.True(current.value == numbers[i]);
            current = current.next;
        }
    }

    [Fact]
    public void RemoveTest()
    {
        SudokuC_.LinkedList<int> SudokuC = new();
        int previous = 0;
        for(int i = 1; i < 10; i++)
        {
            Assert.True(SudokuC.Insert(i, previous));
            previous = i;
        }

        for(int i = 1; i < 10; i++)
        {
            SudokuC.head = SudokuC.Remove(i, SudokuC.head);
            SudokuC.Remove(i, SudokuC.head);
            Node<int> current = SudokuC.head;
            for(int j = 0; j < 9 - i; j++)
            {
                Assert.True(current.value != i);
                current = current.next;
            }
        }
    }


    public static TheoryData<List<int[]>, List<List<int>>, List<List<int>>> Grids => new()
    {
        {new List<int[]> {new int[]{3, 0, 0}, new int[]{0, 1, 0}, new int[]{1, 0, 2} }, 
        new List<List<int>> {new List<int>{1, 2}, new List<int>{2, 3}, new List<int>{3} }, 
        new List<List<int>> {new List<int>{1, 2}, new List<int>{0, 2}, new List<int>{1} }}
    };
    
    [Theory]
    [MemberData(nameof(Grids))]
    public void GenerationTest(List<int[]> setup, List<List<int>> remaining, List<List<int>> indexes)
    {
        int[] allowedNumbers = {1, 2, 3};
        Sudoku sudoku = new(setup, allowedNumbers);

        (List<int[]> gathered, int[] number) = (new List<int[]>(), new int[allowedNumbers.Length]);
        for(int i = 0; i < remaining.Count; i++)
        {
            sudoku.GenerateNumber(indexes[i], remaining[i], gathered, number, 0);
            
            foreach(int[] possibility in gathered)
            {
                Assert.Contains(allowedNumbers[i], possibility);
            }
            gathered.Clear();
            number = new int[allowedNumbers.Length];
        }
    }

    [Theory]
    [MemberData(nameof(Grids))]
    public void PossibilityTest(List<int[]> setup, List<List<int>> belh, List<List<int>> balh)
    {
        int[] allowedNumbers = {1, 2, 3};
        Sudoku sudoku = new(setup, allowedNumbers);

        (List<int> remaining, List<int> indexes) = (new List<int>(), new List<int>());

        sudoku.SetupPossibilities(remaining, indexes);
        Node<int[]> current = sudoku.possibilities.head;
        while(current != sudoku.possibilities.tail)
        {
            for(int i = 0; i < allowedNumbers.Length; i++)
            {
                Assert.Contains(allowedNumbers[i], current.value);
            }
            current = current.next;
        }
    }
}
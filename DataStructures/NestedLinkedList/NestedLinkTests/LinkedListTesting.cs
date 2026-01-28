namespace NestedLinkTests;

using System.Globalization;
using NestedLinkedList;

public class LinkedListTesting
{
    [Theory]
    [InlineData(new int[] { 1, 2, 3 }, new int[] { 4, 5 }, new int[] { 6 }, new int[] {1, 2, 3, 4, 5, 6})]
    [InlineData(new int[] { 4, 12, 7 }, new int[] { 3, 8, 9, 10 }, new int[] { 11, 15 }, new int[] {4, 12, 7, 3, 8, 9, 10, 11, 15})]
    public void ValueTesting(int[] first, int[] second, int[] third, int[] expected)
    {
        // Random random = new Random();
        // LinkedList original = new LinkedList();
        // LinkedList list = original;

        // foreach(int val in array)
        // {
        //     Node test = list.Head;
        //     for(int i = 0; i < val; i++)
        //     {
        //         test.value = random.Next();
        //         test.next = new Node();
        //         test = test.next;
        //     }

        //     test.child = new LinkedList();
        //     list = test.child;
        // }

        LinkedList original = new LinkedList();
        LinkedList test = original;
        Node current = test.Head;
        int[][] array = {first, second, third};

        for(int j = 0; j < 3; j++)
        {
            for(int i = 0; i < array[j].Length; i++)
            {
                current.value = array[j][i];

                if(i == array[j].Length - 1) continue;
                current.next = new Node();
                current = current.next;
            }

            current.child = new LinkedList();
            test = current.child;
            current = test.Head;
        }

        LinkedList result = original.NewGetValue();
        Node resNode = result.Head;

        foreach(int i in expected)
        {
            Assert.Equal(i, resNode.value);
            resNode = resNode.next;
        }
    }
}
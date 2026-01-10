using BSTHeapCombo;

namespace BSTHeapTesting;

public class BSTHeapTests
{
    [Theory]
    [InlineData(5, 3, 7, 19, 23, 45, 1)]
    [InlineData(10, 3, 7, 2, 5, 0, 48, 39)]
    public void InsertTest(params int[] array)
    {
        BSTHeap<int> tree = new BSTHeap<int>();
        foreach (int i in array)
        {
            tree.Head = tree.Insert(i, tree.Head);
            Assert.True(tree.Search(i, tree.Head).value == i);
        }

        Assert.True(tree.TestPriorityAndValue(tree.Head));
    }

    
    [Theory]
    [InlineData(5, 3, 7, 19, 23, 45, 1)]
    [InlineData(10, 3, 7, 2, 5, 0, 48, 39)]
    public void RemoveTest(params int[] array)
    {
        BSTHeap<int> tree = new BSTHeap<int>();
        foreach (int i in array)
        {
            tree.Insert(i, tree.Head);
        }

        foreach(int i in array)
        {
            tree.Remove(i, tree.Head);
            Assert.True(tree.TestPriorityAndValue(tree.Head));
            Assert.True(tree.Search(i, tree.Head) == null);
        }
    }
}
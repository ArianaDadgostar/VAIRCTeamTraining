namespace BSTHeapCombo;

public class Node<T> where T : IComparable<T>
{
    public Node<T> left;
    public Node<T> right;
    public T value;
    public int priority;

    public Node(T value, int priority)
    {
        this.value = value;
        this.priority = priority;
    }
}

public class BSTHeap <T> where T : IComparable<T>
{
    public Node<T> Head;

    public Node<T> RotateLeft(Node<T> node)
    {
        Node<T> newParent = node.right;
        newParent.left = node;
        node.right = newParent.left;

        return node;
    }

    public Node<T> RotateRight(Node<T> node)
    {
        Node<T> newParent = node.left;
        newParent.right = node;
        node.left = newParent.right;

        return node;
    }

    public Node<T> Insert(T value, Node<T> node)
    {
        if(node == null)
        {
            Random random = new Random();
            node = new Node<T>(value, random.Next(1, 100));
            return node;
        }

        if(value.CompareTo(node.value) > 0)
        {
            node.right = Insert(value, node.right);
        }
        else if(value.CompareTo(node.value) < 0)
        {
            node.left = Insert(value, node.left);
        }

        if((node.left == null || node.left.priority >= node.priority)
            && (node.right == null || node.right.priority >= node.priority)) return node;

        if(node.left != null && node.left.priority < node.priority)
        {
            node = RotateRight(node);
            return node;
        }
        
        node = RotateLeft(node);
        return node;
    }

    public bool TestPriorityAndValue(Node<T> node)
    {
        if(node == null) return true;

        if((node.left != null && node.left.priority < node.priority)
            || (node.right != null && node.right.priority < node.priority)) return false;

        if((node.left != null && node.left.value.CompareTo(node.value) > 0) 
            || (node.right != null && node.right.value.CompareTo(node.value) < 0)) return false;

        return TestPriorityAndValue(node.left) && TestPriorityAndValue(node.right);
    }
}
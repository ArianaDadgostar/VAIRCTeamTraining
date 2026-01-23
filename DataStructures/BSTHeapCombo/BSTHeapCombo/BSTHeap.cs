using System.Diagnostics.CodeAnalysis;

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
        Node<T> newLeft = node.right.left;
        Node<T> newParent = node.right;
        newParent.left = node;
        node.right = newLeft;

        return newParent;
    }

    public Node<T> RotateRight(Node<T> node)
    {
        Node<T> newRight = node.left.right;
        Node<T> newParent = node.left;
        newParent.right = node;
        node.left = newRight;

        return newParent;
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

    public void RemovalRemainsInsert(Node<T> node)
    {
        if(node == null) return;

        Head = Insert(node.value, Head);
        RemovalRemainsInsert(node.left);
        RemovalRemainsInsert(node.right);
    }

    public void HeapifyUp(Node<T> node, Node<T> parent)
    {
        Node<T> replacement;
        if(node.left != null && node.right != null)
        {
            if(node.left.priority < node.right.priority)
            {
                replacement = node.left;
            }
            else
            {
                replacement = node.right;
            }
        }
        else if(node.left != null)
        {
            replacement = node.left;
        }
        else
        {
            replacement = node.right;
        }

        parent = node;
        if(replacement == null) return;

        HeapifyUp(replacement, node);
    }

    public Node<T> RemoveChecks(Node<T> node)
    {
        if(node == null) return node;

        if(node.left != null && node.right != null)
        {
            Node<T> nodeReplacement;
            if(node.left.priority <= node.right.priority)
            {


                return node;
            }

            nodeReplacement = node.left;
            node = node.right;
            RemovalRemainsInsert(node.left);
            node.left = nodeReplacement;
            return node;
        }
        else if(node.left != null)
        {
            node = node.left;
        }
        else if(node.right != null)
        {
            node = node.right;
        }

        return node;
    }

    public Node<T> Remove(T value, Node<T> node)
    {
        if(node == null) return node;

        if(value.CompareTo(node.value) > 0)
        {
            node.right = Remove(value, node.right);
        }
        else if(value.CompareTo(node.value) < 0)
        {
            node.left = Remove(value, node.left);
        }
        else if(!node.value.Equals(value)) return node;

        node = NewRemoveChecks(node);

        return node;
    }

    public Node<T> NewRemoveChecks(Node<T> node)
    {
        node.priority = 100000;

        node = TravelDown(node);

        node = null;
        return node;
    }

    public Node<T> TravelDown(Node<T> node)
    {
        if((node.left == null || node.left.priority >= node.priority)
            && (node.right == null || node.right.priority >= node.priority)) return node;

        if(node.left != null && node.left.priority < node.priority)
        {
            node = RotateRight(node);
        }
        else
        {
            node = RotateLeft(node);
        }

        return TravelDown(node);
    }

    public Node<T> Search(T value, Node<T> node)
    {
        if(node == null) return null;

        if(value.CompareTo(node.value) > 0)
        {
            return Search(value, node.right);
        }
        else if(value.CompareTo(node.value) < 0)
        {
            return Search(value, node.left);
        }
        else return node;
    }

    #region Testing

    public bool TestPriorityAndValue(Node<T> node)
    {
        if(node == null) return true;

        if((node.left != null && node.left.priority < node.priority)
            || (node.right != null && node.right.priority < node.priority)) return false;

        if((node.left != null && node.left.value.CompareTo(node.value) > 0) 
            || (node.right != null && node.right.value.CompareTo(node.value) < 0)) return false;

        return TestPriorityAndValue(node.left) && TestPriorityAndValue(node.right);
    }

    #endregion
}
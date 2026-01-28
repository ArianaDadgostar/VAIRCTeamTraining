using System.Runtime.Serialization.Formatters;

namespace NestedLinkedList;

public class Node
{
    public int value;
    public LinkedList child;
    public Node next;
}

public class LinkedList
{
    public Node Head;

    public LinkedList()
    {
        Head = new Node();
    }

    public void PlaceChildren(ref Node node, ref Node previous)
    {
        Node test = node;
        while(test.next != null)
        {
            test = test.next;
            if(test.child != null)
            {
                PlaceChildren(ref test.child.Head, ref test);
            }
        }

        test.next = previous.next;
        previous.next = node;
    }

    public LinkedList GetValue()
    {
        Node test = Head;
        while(test.next != null)
        {
            test = test.next;
            if(test.child != null)
            {
                PlaceChildren(ref test.child.Head, ref test);
            }
        }

        return this;
    }

    public LinkedList NewGetValue()
    {
        Node test = Head;
        Node next = null;
        while(test != null || next != null)
        {
            if(test.child != null)
            {
                next = test.next;
                test.next = test.child.Head;
                test.child = null;
            }

            if(test.next == null && next != null)
            {
                test.next = next;
                next = null;
            }
            
            test = test.next;
        }
        return this;
    }
}

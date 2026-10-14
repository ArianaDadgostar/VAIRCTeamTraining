// See https://aka.ms/new-console-template for more information

using System;

namespace LinkedListExperiment
{
    class Node<T> where T : IComparable<T>
    {
        public T value;
        public Node<T> previous;
        public Node<T> next;

        public Node(T value, Node<T> previous, Node<T> next)
        {
            this.value = value;
            this.previous = previous;
            this.next = next;
        }
    }

    class LinkedList<T> where T : IComparable<T>
    {
        Node<T> head;
        Node<T> tail;
        Stack<Node<T>> Removed;

        public LinkedList()
        {
            Removed = new Stack<Node<T>>();
        }

        public void Insert(T value,  T previous)
        {
            if(tail == null && head != null)
            {
                tail = new Node<T>(value, head, head);
                head.previous = tail;
                head.next = tail;
            }

            head = (head == null) ? new Node<T>(value, null, null) : head;
            
            if(head.value.Equals(value) || tail.value.Equals(value)) return;

            Node<T> current = head;
            while(current != tail)
            {
                if(!current.value.Equals(previous))
                {
                    current = current.next;
                    continue;
                }
                
                Node<T> newNode = new Node<T> (value, current, current.next);
                current.next.previous = newNode;
                current.next = newNode;
                return;
            }

            Node<T> node = new Node<T> (value, tail, head);
            head = node;
            tail.next = node;
            head.next.previous = head;
        }

        public void Remove(T value)
        {
            Node<T> current = head;
            while(current != tail)
            {
                if(!current.value.Equals(value))
                {
                    current = current.next;
                    continue;
                }
                
                Removed.Push(current);
                current.next.previous = current.previous;
                current.previous.next = current.next;
                return;
            }
        }

        public void Print()
        {
            Node<T> current = head;
            while(current != tail)
            {
                Console.WriteLine(current.value + ", ");
                current = current.next;
            }
            Console.WriteLine(current.value);
        }

        public void Undo()
        {
            Node<T> recent = Removed.Pop();

            Node<T> current = head;
            while(current != tail)
            {
                if(current != recent.previous) continue;
                
                current.next.previous = recent;
                current.next = recent;
                return;
            }
        }
    }

    public class Program
    {
        List<uint> elements = new List<uint>();

        static void Actions(LinkedList<int> linkedList)
        {
            Console.WriteLine("What do you wish to do?");

            string output = Console.ReadLine() ?? "";
            switch(output)
            {
                case "add":
                    {
                        Console.WriteLine("What value do you want to add? ");
                        int value = int.Parse(Console.ReadLine() ?? "");
                        Console.WriteLine("What is previous? ");
                        int previous = int.Parse(Console.ReadLine() ?? "");
                        
                        linkedList.Insert(value, previous);
                        break;
                    }

                case "remove":
                    {
                        Console.WriteLine("What value do you want to remove? ");
                        int value = int.Parse(Console.ReadLine() ?? "");
                        
                        linkedList.Remove(value);
                        break;
                    }

                case "undo":
                    {
                        linkedList.Undo();
                        break;
                    }

                case "print":
                    {
                        linkedList.Print();
                        break;
                    }
            }
        }

        static int Main(string[] args)
        {
            LinkedList<int> linkedList = new LinkedList<int>();
            while(true)
            {
                Actions(linkedList);
            }
        }
    }

}
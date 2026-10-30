// See https://aka.ms/new-console-template for more information

using System;
using System.Security.Principal;

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
        public Node<T> head;
        public Node<T> tail;
        public Stack<Node<T>> Removed;

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

        public Node<T> Remove(T value, Node<T> node)
        {
            Node<T> current = node;
            if(head == tail || head == null)
            {
                Removed.Push(current);
                return null;
            }
            if(current == tail) return current;

            if(!current.value.Equals(value))
            {
                current.next = Remove(value, current.next);
                return current;
            }
            
            Removed.Push(current);
            current.next.previous = current.previous;
            current.previous.next = current.next;
            current = current.next;
            return current;
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
            if(Removed.Count == 0) return;
            Node<T> recent = Removed.Pop();

            Node<T> current = head;
            if(current == null)
            {
                head = recent;
                head.next = recent;
                head.previous = recent;
                tail = recent;
                tail.next = recent;
                tail.previous = recent;
                return;
            }
            while(current != tail)
            {
                if(current != recent.next) 
                {
                    if(current == current.next) return;
                    current = current.next;
                    continue;
                }
                head = (head == current) ? recent : head;
                
                current.previous.next = recent;
                current.previous = recent;
                return;
            }
            if(current != recent.next) return;
            current.previous.next = recent;
            current.previous = recent;
            head = recent;
        }
    
        public int GetCount()
        {
            if(head == null) return 0;
            int count = 0;
            Node<T> current = head;
            while(current != tail)
            {
                current = current.next;
                count++;
            }
            count++;
            return count;
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
                        
                        linkedList.head = linkedList.Remove(value, linkedList.head);
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

        static int FindSum(List<int> values)
        {
            int sum = 0;
            foreach(int value in values)
            {
                sum += value;
            }
            return sum;
        }

        static List<int> SearchCombinations(LinkedList<int> linkedList, List<int> chosen, int goal)
        {
            Node<int> current = linkedList.head;
            int sum = FindSum(chosen);
            int removeCount = 0;
            while(linkedList.head != null)
            {
                linkedList.head =linkedList.Remove(current.value, linkedList.head);
                removeCount++;
                if(sum + current.value > goal)
                {
                    current = current.next;
                    continue;
                }
                chosen.Add(current.value);

                if(sum + current.value == goal) return chosen;

                List<int> result = SearchCombinations(linkedList, chosen, goal); // 10/27 CREATION OF TAIL WITHIN UNDO IS FLAWED
                if(result != null) return result;

                chosen.RemoveAt(chosen.Count - 1);
                current = current.next;
            }
            for(int i = 0; i < removeCount; i++)
            {
                linkedList.Undo();
            }
            return null;
        }

        static List<int> TraversePossibilities(LinkedList<int> linkedList, int goal)
        {
            Node<int> current = linkedList.head;
            while(current != linkedList.tail)
            {
                List<int> chosen = new List<int>();
                chosen = SearchCombinations(linkedList, chosen, goal);
                if(chosen != null) return chosen;
                current = current.next;
            }
            return null;
        }

        static int Main(string[] args)
        {
            LinkedList<int> linkedList = new LinkedList<int>();

            linkedList.Insert(147, 0);
            linkedList.Insert(95, 0);
            linkedList.Insert(219, 100);
            linkedList.Insert(130, 219);
            linkedList.Insert(90, 100);
            linkedList.Insert(178, 85);
            linkedList.Insert(240, 178);
            linkedList.Insert(70, 85);

            linkedList.Print();
            List<int> result = TraversePossibilities(linkedList, 255);
            if(result != null)
            {
                Console.WriteLine("Found a combination that sums to 500: ");
                foreach(int value in result)
                {
                    Console.WriteLine(value);
                }
            }
            else
            {
                Console.WriteLine("No combination found that sums to 500.");
            }

            return 0;
        }
    }

}
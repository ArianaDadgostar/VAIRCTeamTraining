using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace SudokuC_;

public class Node<T>
{
    public T value;
    public Node<T> previous;
    public Node<T> next;

    public Node(T value)
    {
        this.value = value;
    }
}

public class LinkedList<T> where T : IComparable<T>
{
    public Node<T> head;
    public Node<T> tail;
    public Stack<Node<T>> Removed;

    public bool Insert(T value, T previous)
    {
        Node<T> addition = new(value);
        Node<T> current = head;

        if(head is null)
        {
            head = head.next = head.previous = addition;
            return true;
        }

        while(true)
        {
            if(current.value.Equals(previous)) break;

            if(current == tail) return false;

            current = current.next;
        }

        tail ??= addition;

        current.next.next.previous = addition;
        addition.next = current.next.next;
        current.next = addition;
        addition.previous = current;

        if(current != tail) return true;

        tail = addition;

        return true;
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

    public void Undo()
    {
        if(Removed.Count == 0) return;
        Node<T> recent = Removed.Pop();

        Node<T> current = head;
        if(current == null)
        {
            head = head.next = head.previous = recent;
            tail = tail.next = tail.previous = recent;
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
            
            current.previous.next = current.previous = recent;
            return;
        }
        if(current != recent.next) return;

        current.previous.next = current.previous = recent;
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
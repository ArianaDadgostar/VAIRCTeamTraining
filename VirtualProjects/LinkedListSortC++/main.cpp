#include <iostream>

using namespace std;

template<typename T>
class Node
{
    public:
        T value;
        Node<T> previous;
        Node<T> next;

        Node(T value, Node<T> previous, Node<T> next)
        {
            this->value = value;
            this->previous = previous;
            this->next = next;
        }
};

template<typename T>
class LinkedList
{
    public:
        Node<T> head;
        Node<T> tail;
        std::stack<Node<T>> Removed;

     LinkedList()
    {
        Removed = new Stack<Node<T>>();
    }

    void Insert(T value,  T previous)
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

    
};

int main() {

    LinkedList<int> list;
    list.Insert(1, 0);
    list.Insert(2, 1);
    list.Remove(1, list.head);
    list.Undo();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
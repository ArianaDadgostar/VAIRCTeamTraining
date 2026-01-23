// LinkedListDemoC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>

struct Node
{
    int value;
    Node* next;
};

struct LinkedList
{
    Node* head;
    Node* tail;
    size_t length;

    LinkedList()
    {
        head = nullptr;
        tail = nullptr;
        length = 0;
	}

    void RemoveNode(int val)
    {
        if (val > length) return;

        length--;

        Node* previous = head;
        Node* current = head;

        for (size_t i = 0; i < val; i++)
        {
            previous = current;
            current = current->next;
        }

        if (current == head)
        {
            head = current->next;
            delete current;
            return;
        }

        if (current == tail)
        {
            tail = previous;
            previous->next = nullptr;
            delete current;
            return;
        }

        previous->next = current->next;
        delete current;
        return;
    }

    bool Remove(int val)
    {
		if (length == 0) return false;
        Node* previous = nullptr;
		Node* current = head;
        for (size_t i = 0; i < length; i++)
        {
            if (current->value == val) break;
			if (current->next == nullptr) return false;
			previous = current;
			current = current->next;
        }

        length--;

        if (previous == nullptr)
        {
            head = current->next;
            delete current;
			return true;
        }

		previous->next = current->next;
		delete current;
        return true;
    }

    void AddLast(int val)
    {
        length++;

        Node* newNode = new Node();
        newNode->value = val;
        newNode->next = nullptr;
        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
            return;
        }

        tail->next = newNode;
        tail = newNode;
    }

    void AddFirst(int val)
    {
        length++;

        Node* newNode = new Node();
        newNode->value = val;
        newNode->next = nullptr;
        if (head == nullptr)
        {
            tail = newNode;
        }
        newNode->next = head;
		head = newNode;
    }
};

int main()
{    
    LinkedList linkedList{};
    srand(time(0));

    for (size_t i = 0; i < 10; i++)
    {
        linkedList.AddLast(i);
        std::cout << linkedList.length;
    }

    while (true)
    {
        /*std::cout << "Enter node value";
        std::cin >> current.value;
        std::string output;
        std::cout << "Continue? (T/F)";
		std::getline(std::cin, output);
        if (output == "F" || output == "f")
        {
            break;
		}

        LinkedList nextNode;
        current.next = &nextNode;
		current = nextNode;*/

		int value = rand() % 10;

        linkedList.RemoveNode(value);

		linkedList.AddLast(value);
        std::cout << linkedList.length;
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

/*

    "New" operand is required for a pointer, but allocates memory on the heap if used.
	If scope ends before pointer is deleted, memory leak occurs.

    NOTE: Pointer --> handle of a pan
		  Data type --> pan
		  When scope ends, handle disappears, but pan remains until explicitly deleted. Pan is hot, so no touching it.

*/

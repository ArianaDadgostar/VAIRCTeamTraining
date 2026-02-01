#include <iostream>

template<typename T>
struct Node;

template<typename T>
struct LinkedList;

template<typename T>
struct Node
{
    public:
        T value;
        Node<T>* next;
        LinkedList<T>* child;

    Node(T value)
    {
        this->value = value;
        this->next = nullptr;
        this->child = nullptr;
    }
};

template<typename T>
struct LinkedList
{
    Node<T>* Head;

    LinkedList()
    {
        Head = nullptr;
    }
    
    void PlaceChildren(Node<T>* node, Node<T>* previous)
    {
        Node<T>* test = node;
        while(test->next != nullptr)
        {
            test = test->next;
            if(test->child != nullptr)
            {
                PlaceChildren(test->child->Head, test);
            }
        }

        test->next = previous->next;
        previous->next = node;
    }

    LinkedList GetValue()
    {
        Node<T>* test = Head;
        while(test->next != nullptr)
        {
            test = test->next;
            if(test->child != nullptr)
            {
                PlaceChildren(test->child->Head, test);
            }
        }

        return this;
    }

    LinkedList NewGetValue()
    {
        Node<T>* test = Head;
        Node<T>* next = nullptr;
        while(test != nullptr || next != nullptr)
        {
            if(test->child != nullptr)
            {
                next = test->next;
                test->next = test->child->Head;
                test->child = nullptr;
            }

            if(test->next == nullptr && next != nullptr)
            {
                test->next = next;
                next = nullptr;
            }
            
            test = test->next;
        }
        return *this;
    }

    void AddChild(Node<T>* node, T value)
    {
        if(node->child == nullptr)
        {
            node->child = new LinkedList<T>{};
            node->child->Head = new Node<T>{value};
        }
        else
        {
            Node<T>* temp = node->child->Head;
            while(temp->next != nullptr)
            {
                temp = temp->next;
            }
            temp->next = new Node<T>{value};
        }
    }
};

void PrintList(Node<int>* head, int level)
{
    Node<int>* temp = head;
    while(temp != nullptr)
    {
        for(int i = 0; i < level; i++)
        {
            std::cout << "  ";
        }
        std::cout << temp->value << "\n";
        if(temp->child != nullptr)
        {
            PrintList(temp->child->Head, level + 1);
        }
        temp = temp->next;
    }
}

int main() 
{
    srand(time(0));

    LinkedList<int> list = LinkedList<int>();
    list.Head = new Node<int>(1);
    Node<int>* head = list.Head;

    // Create main list
    for (int i = 2; i <= 8; i++) 
    {
        head->next = new Node<int>{i};
        head = head->next;
    }

    // Add random children
    Node<int>* temp = list.Head;
    while (temp->next != nullptr) {
        int childCount = rand() % 4; // 0 to 3 children
        for (int i = 0; i < childCount; i++) {
            int value = rand() % 100;
            list.AddChild(temp, value);
        }
        temp = temp->next;
    }

    PrintList(list.Head, 0);

    list.NewGetValue();

    std::cout << "Nested Linked List:\n" << " ";
    temp = list.Head;
    while (temp != nullptr) 
    {
        std::cout << temp->value << " ";
        temp = temp->next;
    }

    return 0;
}

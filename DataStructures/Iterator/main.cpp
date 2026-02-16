#include <iostream>

struct LinkedListNode{
    public:
        int value;
        LinkedListNode* next;
};

struct LinkedList{
    LinkedListNode* head;
    LinkedListNode* tail;

    public: 

    struct Iterator
    {
        LinkedListNode* current;
        
        Iterator& operator++()
        {
            current = current->next;
            return *this;
        }

        int& operator*()
        {
            return current->value;
        }

        bool operator!=(Iterator other)
        {
            return current != other.current;
        }
    };

    Iterator begin()
    {
        return Iterator{head};
    };

    Iterator end()
    {
        return Iterator{nullptr};
    };
};

void RunStuff(LinkedList* list)
{
    for(auto value : *list)
    {
        std::cout << value << std::endl;
    }
}

int main() {
    LinkedList list;
    list.head = new LinkedListNode{14, nullptr};
    list.head->next = new LinkedListNode{21, nullptr};
    list.head->next->next = new LinkedListNode{3, nullptr};
    RunStuff(&list);
}
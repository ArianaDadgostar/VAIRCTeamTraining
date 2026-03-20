#include <iostream>
#include <string>
#include <array>

struct Alien
{
    double* copies;
    double age;
    std::string name;
    
    Alien(double age, std::string name)
    {
        this->age = age;
        this->name = name;
        this->copies = new double(1);
    }

    void operator=(Alien &other)
    {
        this->age = other.age;
        this->name = other.name;
        (*other.copies)++;
        this->copies = other.copies;
    }

    void operator~()
    {
        if(*copies == 1) delete this;

        (*copies) --;
    }
};

template<typename T>
struct Node
{
    T value;
    Node<T>* next = nullptr;
};

struct LinkedList
{
    std::byte* nodes;
    static const int TOTAL_SIZE = 100;
    static const int BLOCK_SIZE = 4;
    static const int TRACKING_SIZE = TOTAL_SIZE/(BLOCK_SIZE + 1);

    template<typename T>
    T* allocate(T item)
    {
        int index = -1;
        for(int i = 0; i < TRACKING_SIZE - sizeof(item); i ++)
        {
            int size = sizeof(item);
            size /= 4;
            if(sizeof(item) % 4 > 0)
            {
                size++;
            }
            for(int j = 0; j < sizeof(item); j ++)
            {
                if(nodes[i + j] == (std::byte)0) break;
                
                if(j == sizeof(item) - 1)
                {
                    index = i;
                }
            }
        }

        std::memcpy(nodes[index], &item, sizeof(item));
    }


};


int main() 
{
    LinkedList list = LinkedList();

    int num = 4;
    list.allocate(num);
}
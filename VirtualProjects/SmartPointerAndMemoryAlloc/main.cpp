#include <iostream>
#include <string>
#include <array>

#define BYTE_TRACKING 0
#define BIT_TRACKING 1

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
    static const int TOTAL_SIZE = 99;
    static const int BLOCK_SIZE = 4;
    static const int TRACKING_SIZE = 11;
    static const int STORING_SIZE = TOTAL_SIZE - TRACKING_SIZE;

    LinkedList()
    {
        nodes = new std::byte[TOTAL_SIZE];
        for(int i = 0; i < TOTAL_SIZE; i ++)
        {
            nodes[i] = (std::byte)0;
        }
    }

#if BIT_TRACKING

    template<typename T>
    bool allocate(T item)
    {
        int size = sizeof(item) / 4;
        size = (sizeof(item) % 4 > 0) ? size : size + 1;

        for(int i = 0; i < TRACKING_SIZE; i ++) //std::ceil(((TRACKING_SIZE * 8) - size) / 8); i ++)
        {
            for(int j = 0; j < (8 - size); j ++)
            {
                std::byte mask = (std::byte)1; // change ts to include # of blocks needed
                mask <<= j;

                if((nodes[STORING_SIZE + i] & mask) == mask) continue;

                nodes[STORING_SIZE + i] |= mask;

                std::memcpy(&nodes[(i * 8) + j], &item, sizeof(item));
            }
        }
    }

#endif

#if BYTE_TRACKING

    static const int TRACKING_SIZE = TOTAL_SIZE/(BLOCK_SIZE + 1);
    static const int STORING_SIZE = TOTAL_SIZE - TRACKING_SIZE;

    template<typename T>
    bool allocate(T item)
    {
        int index = -1;
        int size = sizeof(item);
        size /= 4;

        if(sizeof(item) % 4 > 0)
        {
            size++;
        }

        for(int i = STORING_SIZE; i < TOTAL_SIZE - size; i ++)
        {
            for(int j = 0; j < size; j ++)
            {
                if(nodes[i + j] == (std::byte)1) break;
                
                if(j == size - 1)
                {
                    index = i - STORING_SIZE;
                    break;
                }
            }
            if(index >= 0) break;
        }

        if(index < 0) return false;

        std::memcpy(&nodes[index], &item, sizeof(item));

        for(int i = STORING_SIZE + index; i < STORING_SIZE + index + size; i ++)
        {
            nodes[i] = (std::byte)1;
        }

        return true;
    }

    template<typename T>
    bool deAllocate(T item)
    {
        int index = -1;
        int size = sizeof(item);
        size /= 4;

        if(sizeof(item) % 4 > 0)
        {
            size++;
        }

        for(int i = STORING_SIZE; i < TOTAL_SIZE - size; i ++)
        {
            for(int j = 0; j < size; j++)
            {
                if(nodes[i + j] == (std::byte)0) break;

                if(j == size - 1)
                {
                    index = i - STORING_SIZE;
                    break;
                }
            }
            
            T testVal;
            if(std::memcpy(&testVal, &nodes[index], sizeof(T)) == nullptr) break;
        }

        if(index < 0) return false;

        for(int i = STORING_SIZE + index / 4; i < STORING_SIZE + (index / 4) + size; i ++)
        {
            std::memset(&nodes[index], 0, sizeof(item));
            nodes[i] = (std::byte)0;
        }
        return true;
    }

#endif

    void printSlots()
    {
        for(int i = STORING_SIZE; i < TOTAL_SIZE; i ++)
        {
            std::cout << (int)nodes[i] << " ";
        }
        std::cout << std::endl;
    }
};


int main() 
{
    LinkedList list = LinkedList();

    int num = 4;
    long num2 = 5;
    list.allocate(num);
    //list.allocate(num2);
    //list.printSlots();
    //list.deAllocate(num);
    //list.printSlots();
}
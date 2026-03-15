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

template<typename T>
struct LinkedList
{
    int length = 8;
    std::array<Node<T>, 8> nodes;
    std::byte used;

    LinkedList(T headVal)
    {
        nodes[0].value = headVal;
        std::byte current = std::byte{1};
        for(int i = 1; i < length; i ++)
        {
            nodes[i - 1].next = &nodes[i];
            current << 1;
        }
        
        used xor current;
    }

    void add(T val)
    {
        for(int i = 0; i < length; i++)
        {
            if(used[i] == true) continue;
            nodes[i].value = val;
            used[i] = true;
            return;
        }  
    }

    void remove(T val)
    {
        for(int i = 0; i < length; i++)
        {
            if(nodes[i].value != val) continue;

            nodes[i].value = T();
            nodes[i].next = nullptr;
            used[i] = false;
            return;
        }
    }
};

int main() 
{
    Alien original(1000, "Jimmy");

    std::cout<< "Copies: " << *original.copies << std::endl;
    Alien copy1 = Alien(1000, "Jimmy");
    copy1 = original;
    std::cout<< "Copies: " << *original.copies << std::endl;
    Alien copy2 = Alien(1000, "Jimmy");
    copy2  = original;
    std::cout<< "Copies: " << *original.copies << std::endl;

    ~copy1;
    std::cout<< "Copies: " << *original.copies << std::endl;
    ~copy2;
    std::cout<< "Copies: " << *original.copies << std::endl;

    LinkedList<int> list(0);
    for(int i = 1; i < list.length; i++)
    {
        list.add(i);
    }

    list.remove(2);
    list.add(17);

    for(Node node: list.nodes)
    {
        std::cout<< node.value << std::endl;
    }
}
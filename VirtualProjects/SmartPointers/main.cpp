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
    std::byte[] nodes;
};


int main() 
{
}
#include <iostream>
#include <string>

struct Alien
{
    double* copies;
    double age;
    std::string name;
    
    Alien(double age, std::string name)
    {
        this->age = age;
        this->name = name;
        this->copies = new int(1);
    }

    void operator=(Alien &other)
    {
        this->copies = other.copies;
        this->name = other.name;
        this->copies = other.copies;
    }

    void operator~()
    {
        if(*copies == 1) delete this;

        copies --;
    }
};

int main() 
{
    Alien original(1000, "Jimmy");

}
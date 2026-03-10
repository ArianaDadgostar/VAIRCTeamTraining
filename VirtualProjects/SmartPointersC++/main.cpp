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
}
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <list>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>

// See https://aka.ms/new-console-template for more information

struct Analysis
{
    public:
        std::unordered_map<std::string, std::vector<std::string>> storage;
        int maxWordCount;
        int currentWordCount;
        std::string last;
        std::string generated;

    Analysis()
    {
        storage = std::unordered_map<std::string, std::vector<std::string>>();
        last = "";
        generated = "";
        currentWordCount = 0;
    }

    void Establish(std::string File)
    {
        std::string previous = "";
        std::string current = "";

        for(char var : File)
        {
            if(var != ' ')
            {
                current += var;
                continue;
            }

            if(previous == "")
            {
                previous = current;
                current = "";

                storage.emplace(previous, std::vector<std::string>());

                continue;
            }

            storage[previous].push_back(current);

            previous = current;
            current = "";

            if(storage.find(previous) != storage.end()) continue;


            storage.emplace(previous, std::vector<std::string>());
        }
    }

    bool CanGenerate()
    {
        if(last == "") return false;

        generated += " ";
    
        int random_number = std::rand() % 100 + 1;

        auto& vec = storage.at(last);
        std::string current = vec[random_number % vec.size()];

        generated += current;

        last = current; 
        currentWordCount++;

        return true;
    }
};

int main() {
    std::ifstream file("file.txt");
    std::string fileContent;

    if(!file.is_open()) {
        std::cout << "Error opening file.txt" << std::endl;
        return 1;
    }

    std::ifstream ifs(fileContent);

    std::stringstream ss;
    ss << ifs.rdbuf(); 
    
    std::string content = ss.str();
    
    ifs.close();

    Analysis analysis = Analysis();
    analysis.maxWordCount = 40;


    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    analysis.Establish(fileContent);

    analysis.last = "Trees";
    analysis.generated += analysis.last;

    while(analysis.CanGenerate() && analysis.currentWordCount <= analysis.maxWordCount){}

    std::cout << analysis.generated << std::endl;

    return 0;
}
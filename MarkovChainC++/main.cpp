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
        std::string first = "";
        std::string second = "";
        std::string current = "";

        for(char var : File)
        {
            if(var != ' ')
            {
                current += var;
                continue;
            }

            if(first.empty())
            {
                first = current;
                current = "";
                continue;
            }
            else if(second.empty())
            {
                second = current;
                current = "";
                continue;
            }

            storage[first + second].push_back(current);

            first = second;
            second = current;
            current = "";

            if(storage.find(first + second) != storage.end()) continue;


            storage.emplace(first + second, std::vector<std::string>());
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

    std::string line;
    std::cout << "File Contents:" << std::endl;

    // 3. Read the file line by line using std::getline
    while (std::getline(file, line)) {
        fileContent += line + " ";
    }

    // 4. Close the file (optional but good practice)
    file.close();

    Analysis analysis = Analysis();
    analysis.maxWordCount = 40;


    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    analysis.Establish(fileContent);

    analysis.last = "Harry is";
    analysis.generated += analysis.last;

    while(analysis.CanGenerate() && analysis.currentWordCount <= analysis.maxWordCount){}

    std::cout << analysis.generated << std::endl;
}
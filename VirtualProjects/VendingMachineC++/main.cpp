#include <iostream>
#include <map>
#include <vector>
using namespace std;

class VendingMachine
{
    public:
        map<int, int> coins =
        {
            {1, 1},
            {10, 5},
            {25, 1}
        };

        map<int, vector<int>> dynamic;
        
        int coinCount;

        bool ContainsCoin(int coinAmount)
        {
            cout << coinAmount<< endl;
            auto key = coins.find(coinAmount);
            if(key != coins.end()) return true;
            cout << "bru rlly" << endl;
            return false;
        }

        void AssessValue(int coinAmount)
        {
            int index;
            int smallestLength = 1000;
            for(auto v = coins.begin(); v != coins.end(); v++)
            {
                cout << "here" << endl;
                auto result = (dynamic.find(coinAmount - v->first));
                vector<int> val = (result != dynamic.end()) ? result->second : vector<int>{};
                cout << "here2" << endl;

                if(val == vector<int>{}) continue;

                cout << "here3" << endl;
                
                if(val.size() < smallestLength)
                {
                    smallestLength = val.size() + 1;
                    index = v->first;
                }
            }
            vector<int> newCoins = dynamic.find(index)->second;
            newCoins.push_back(coinAmount - index);
            dynamic.emplace(coinAmount, newCoins);
        }

        void AnalyzePreviousCases(int start, int goal)
        {
            for(int i = start; i <= goal; i++)
            {
                if(ContainsCoin(i))
                {
                    dynamic.emplace(i, vector<int> {i});
                    continue;
                }

                AssessValue(i);
            }
        }

        
};

int main()
{
    VendingMachine machine;
    int change;
    std::cout << "How much change cuh" << endl;
    std::cin >> change;

    machine.AnalyzePreviousCases(1, change);
    vector<int> values = machine.dynamic.find(change)->second;
    for(int i = 0; i < values.size(); i++)
    {
        std::cout << values.at(i) << ", ";
    }
    // Your code here
    return 0;
}
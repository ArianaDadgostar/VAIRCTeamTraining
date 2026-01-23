// BSTHeapCPlus.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <queue>

template<typename T>
struct Node
{
    public:
        Node<T>* left;
        Node<T>* right;
        T value;
        int priority;

        Node(T value, int priority)
        {
            this->value = value;
            this->priority = priority;
        }
};

template<typename T>
struct BSTHeap
{
    Node<T>*Head;

    Node<T>* RotateLeft(Node<T>* node)
    {
        Node<T>* newRight = node->right->left;
        Node<T>* newParent = node->right;
        newParent->left = new Node<T>(node->value, node->priority);
        newParent->left->left = node->left;

        delete node;

        node = newParent;
        node->left->right = newRight;
        //node->right = newLeft;

        return node;
    }


    Node<T>* RotateRight(Node<T>* node)
    {
        Node<T>* newLeft = node->left->right;
        Node<T>* newParent = node->left;
        newParent->right = new Node<T>(node->value, node->priority);
        newParent->right->right = node->right;

        delete node;

        node = newParent;
        node->right->left = newLeft;

        return node;
    }

    Node<T>* Insert(T value, Node<T>* node)
    {
        if (node == nullptr)
        {
            node = new Node<T>(value, rand()%100);
            return node;
        }

        if (value > node->value)
        {
            node->right = Insert(value, node->right);
        }
        else if (value < node->value)
        {
            node->left = Insert(value, node->left);
        }

        if ((node->left == nullptr || node->left->priority >= node->priority)
            && (node->right == nullptr || node->right->priority >= node->priority)) return node;

        if (node->left != nullptr && node->left->priority < node->priority)
        {
            node = RotateRight(node);
            return node;
        }

        node = RotateLeft(node);
        return node;
    }

    void InOrderTraversal(Node<T>* current)
    {
        if (current == nullptr) return;

        InOrderTraversal(current->left);
        std::cout << current->value << " "; // this line somehow makes the value 1
        InOrderTraversal(current->right);
    }


    void LevelOrderTransversal()
    {
        std::queue<Node<T>*> storage = std::queue<Node<T>*>();
        std::queue<int> result = std::queue<int>();
        
        storage.push(Head);
        while (!storage.empty())
        {
            Node<T>* curr = storage.front();
            storage.pop();

            if(curr == nullptr) continue;

            result.push(curr->priority);

            if (curr->left != nullptr)
            {
                storage.push(curr->left);
            }
            if (curr->right != nullptr)
            {
                storage.push(curr->right);
            }
        }

        while (!result.empty())
        {
            std::cout << result.front() << " ";
            result.pop();
        }
    }

    
    void RemovalRemainsInsert(Node<T>* node)
    {
        if(node == nullptr) return;

        Head = Insert(node->value, Head);
        RemovalRemainsInsert(node->left);
        RemovalRemainsInsert(node->right);
    }

    
    Node<T>* NewRemoveChecks(Node<T>* node)
    {
        node->priority = 100000;

        node = TravelDown(node); // returning node, so returning child but also has to so deletion works, so idk?

        return node;
    }

    Node<T>* TravelDown(Node<T>* node)
    {
        if((node->left == nullptr || node->left->priority >= node->priority)
            && (node->right == nullptr || node->right->priority >= node->priority))
            {
                node = nullptr;
                delete node;
                return node;
            }

        if(node->left != nullptr && (node->right == nullptr || node->left->priority < node->right->priority))
        {
            node = RotateRight(node);
            node->right = TravelDown(node->right);
            return node;
        }
        else
        {
            node = RotateLeft(node);
            node->left = TravelDown(node->left);
            return node;
        }
    }

    Node<T>* RemoveChecks(Node<T>* node)
    {
        if(node == nullptr) return node;

        if(node->left != nullptr && node->right != nullptr)
        {
            Node<T>* nodeReplacement;
            if(node->left->priority <= node->right->priority)
            {
                nodeReplacement = node->right;
                node = node->left;
                RemovalRemainsInsert(node->right);
                node->right = nodeReplacement;

                return node;
            }

            nodeReplacement = node->left;
            node = node->right;
            RemovalRemainsInsert(node->left);
            node->left = nodeReplacement;
            return node;
        }
        else if(node->left != nullptr)
        {
            node = node->left;
        }
        else if(node->right != nullptr)
        {
            node = node->right;
        }
        else
        {
            delete node;
            return nullptr;
        }

        return node;
    }

    Node<T>* Remove(T value, Node<T>* node)
    {
        if(node == nullptr) return node;

        if(value > node->value)
        {
            node->right = Remove(value, node->right);
            return node;
        }
        else if(value < node->value)
        {
            node->left = Remove(value, node->left);
            return node;
        }
        else if(node->value != value) return node;

        node = NewRemoveChecks(node);

        return node;
    }
};

int main()
{
    BSTHeap<int> tree = BSTHeap<int>();
    int values[10];
    int set[3] = { 0, 49, 72 };
    srand(time(0)); // seed the random number generator

    for (size_t i = 0; i < 10; i++)
    {
        values[i] = rand() % 100;
        tree.Head = tree.Insert(values[i], tree.Head);
        // tree.InOrderTraversal(tree.Head);
        // std::cout << std::endl;
        // tree.LevelOrderTransversal();
        // std::cout << std::endl;
    }

        std::cout << std::endl;
        std::cout << std::endl;

    for (size_t i = 0; i < 10; i++)
    {
        std::cout << std::endl;
        //tree.LevelOrderTransversal();
        tree.InOrderTraversal(tree.Head);
        tree.Head = tree.Remove(values[i], tree.Head);
    ;
    }
}

/*
0 8 10 12 32 49 55 67 84 99 --> values
24 34 30 93 39 40 47 99 44 83 --> priorities

0 49 72
*/

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

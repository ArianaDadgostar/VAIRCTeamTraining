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
        T* value;
        int priority;

        Node(T value, int priority)
        {
            this->value = &value;
            this->priority = priority;
        }
};

template<typename T>
struct BSTHeap
{
    Node<T>*Head;

    Node<T>* RotateLeft(Node<T>* node)
    {
        Node<T>* newLeft = node->right->left;
        Node<T>* newParent = node->right;
        newParent->left = node;
        node->right = newLeft;

        return newParent;
    }


    Node<T>* RotateRight(Node<T>* node)
    {
        Node<T>* newRight = node->left->right;
        Node<T>* newParent = node->left;
        newParent->right = node;
        node->left = newRight;

        return newParent;
    }

    Node<T>* Insert(T value, Node<T>* node)
    {
        if (node == nullptr)
        {
            node = new Node<T>(value, rand()%100);
            return node;
        }

        if (value > *node->value)
        {
            node->right = Insert(value, node->right);
        }
        else if (value < * node->value)
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
        std::cout << *current->value << " ";
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
};


int main()
{
    BSTHeap<int> tree = BSTHeap<int>();
    int values[9];

    for (size_t i = 0; i < 9; i++)
    {
        values[i] = rand() % 100;
        tree.Head = tree.Insert(values[i], tree.Head);
    }
    //tree.LevelOrderTransversal();
    tree.InOrderTraversal(tree.Head);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

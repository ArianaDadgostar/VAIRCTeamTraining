// BSTDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

struct Node
{
    int value;
    Node* leftChild;
	Node* rightChild;
};

struct BST
{
    Node* root;

    Node* Insert(int val, Node* current)
    {
        if (current == nullptr)
        {
            current = new Node{};
			current->value = val;
            return current;
        }
        if (val == current->value) return current;

        if (val > current->value)
        {
            current->rightChild = Insert(val, current->rightChild);
        }
        else if (val < current->value)
        {
            current->leftChild = Insert(val, current->leftChild);
        }

        return current;
    }

    Node* Remove(int val, Node*& current)
    {
		if (current == nullptr) return nullptr;
        if (val > current->value)
        {
            current->rightChild = Remove(val, current->rightChild);
            return current;
        }
        else if (val < current->value)
        {
            current->leftChild = Remove(val, current->leftChild);
            return current;
        }

        if (val != current->value)
        {
            return current;
        }

        if (current->leftChild != nullptr && current->rightChild != nullptr)
        {
			Node* replacementParent = current->rightChild;
            
            if (replacementParent->leftChild == nullptr)
            {
                Node* leftChildren = current->leftChild;
                current = current->rightChild;
                current->leftChild = leftChildren;
                return current;
            }

            while(replacementParent->leftChild->leftChild != nullptr)
            {
                replacementParent = replacementParent->leftChild;
			}

            Node* original = current;
			current = replacementParent->leftChild;
            replacementParent->leftChild = current->rightChild;
            current->leftChild = original->leftChild;
            current->rightChild = original->rightChild;
        }
        else if (current->leftChild != nullptr)
        {
			current = current->leftChild;
        }
        else if (current->rightChild != nullptr)
        {
            current = current->rightChild;
        }

        return current;
	}

    void InOrderTraversal(Node* current)
    {
        if (current == nullptr) return;

        InOrderTraversal(current->leftChild);
        std::cout << current->value << " ";
		InOrderTraversal(current->rightChild);
    }
};

int main()
{
    BST tree = BST{};
    srand(time(0));

    int values[10];

    for (size_t i = 0; i < 10; i++)
    {
		values[i] = rand() % 100;
        tree.root = tree.Insert(values[i], tree.root);
    }
		tree.InOrderTraversal(tree.root);

    for (size_t i = 0; i < 10; i++)
    {
        tree.Remove(values[i], tree.root);
		tree.InOrderTraversal(tree.root);
		std::cout << std::endl;
    }
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

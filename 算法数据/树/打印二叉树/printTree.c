#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// Data structure to store a Binary Tree node
typedef struct Node
{
    int data;
    struct Node *left, *right;
}Node;

Node *newNode(int data)
{
    Node *node = malloc(sizeof(Node));
    node->data = data;
    node->left = node->right = NULL;

    return node;
}

typedef struct Trunk
{
    struct Trunk *prev;
    char *str;
}Trunk;

Trunk *newTrunk(Trunk *prev, char *str)
{
    Trunk *trunk = malloc(sizeof(Trunk));
    trunk->prev = prev;
    trunk->str = str;
}

// Helper function to print branches of the binary tree
void showTrunks(Trunk *p)
{
    if (p == NULL)
        return;

    showTrunks(p->prev);

    //cout << p->str;
    printf("%s", p->str);
}

// Recursive function to print binary tree
// It uses inorder traversal
void printTree(Node *root, Trunk *prev, bool isLeft)
{
    if (root == NULL)
        return;
    
    char *prev_str = "    ";
    Trunk *trunk = newTrunk(prev, prev_str);

    printTree(root->left, trunk, true);

    if (!prev)
        trunk->str = "---";
    else if (isLeft)
    {
        trunk->str = ".---";
        prev_str = "   |";
    }
    else
    {
        trunk->str = "`---";
        prev->str = prev_str;
    }

    showTrunks(trunk);
    // cout << root->data << endl;
    printf("%d\n",root->data);
    if (prev)
        prev->str = prev_str;
    trunk->str = "   |";

    printTree(root->right, trunk, false);
}

// main function
int main()
{
    Node* root = NULL;

    // Construct above tree    
    root = newNode(1);
    root->left = newNode(2);
    root->right = newNode(3);
    root->left->left = newNode(4);
    root->left->right = newNode(5);
    root->right->left = newNode(6);
    root->right->right = newNode(7);
    root->left->left->left = newNode(8);
    root->left->left->right = newNode(9);
    root->left->right->left = newNode(10);
    root->left->right->right = newNode(11);
    root->right->left->left = newNode(12);
    root->right->left->right = newNode(13);
    root->right->right->left = newNode(14);
    root->right->right->right = newNode(15);
    
    // print constructed binary tree
    printTree(root, NULL, false);

    return 0;
}
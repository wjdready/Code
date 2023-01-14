#include <stdio.h>

typedef struct bitreeNode {
    int data;
    struct bitreeNode *left;
    struct bitreeNode *right; 
} bitreeNode;


// 给定一个 data，创建一个二叉树节点
bitreeNode *newBitreeNode(int data)
{
    bitreeNode *node = (bitreeNode*)malloc(sizeof(bitreeNode));
    if(node == NULL)
        return NULL;
    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return node;
}

int main()
{
    // 创建根节点
    bitreeNode *root = newBitreeNode(1);
    /* 下面是上述语句后的树 
  
        1 
      /   \ 
     NULL  NULL   
    */

    root->left = newBitreeNode(2);
    root->right = newBitreeNode(3);
    /* 2和3分别成为1的左右儿子
           1 
         /   \ 
        2      3 
     /    \    /  \ 
    NULL NULL NULL NULL 
    */
    root->left->left = newBitreeNode(4);
     /* 4成为2的左儿子
           1 
       /       \ 
      2          3 
    /   \       /  \ 
   4    NULL  NULL  NULL 
  /  \ 
NULL NULL 
    */
}
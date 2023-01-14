二叉树|集合1(简介)
========

**树**：与数组、链表、堆栈和队列不同，它们是线性数据结构，树是分层数据结构。

**树的概念**：最上面的节点称为树的根。直接位于元素下的元素称为其子元素。某物正上方的元素称为其父元素。例如，‘a’是‘f’的子代，‘f’是‘a’的父代。最后，没有子元素的元素称为叶子(leaves)。

        tree
        ----
          j    <-- root
        /   \
       f      k  
     /   \      \
    a     h      z    <-- leaves 

**为什么要用树？**
1. 使用树的一个原因可能是您希望存储自然形成层次结构的信息。例如，计算机上的文件系统：

        file system
        -----------
            /    <-- root
        /      \
        ...      home
             /          \
           ugrad        course
            /       /      |     \
        ...      cs101  cs112  cs113  

2. 树(具有一些排序，例如BST)提供适中的访问/搜索(比链表快，比数组慢)。
3. 树提供适度的插入/删除(比数组快，比无序链表慢)。
4. 与链表一样，与数组不同，树没有节点数量的上限，因为节点是使用指针链接的。

**树的主要应用包括**：
1. 操作分层数据。
2. 使信息易于搜索(参见树遍历)。
3. 操作已排序的数据列表。
4. 作为用于合成数字图像以获得视觉效果的工作流。
5. 路由器算法。
6. 多阶段决策的形式(见商业国际象棋)。

**二叉树**：元素最多有2个子元素的树称为二叉树。由于二叉树中的每个元素只能有2个子元素，所以我们通常将它们命名为Left和Right子元素。

**C中的二叉树表示**：树由指向树中最顶层节点的指针表示。如果树为空，则根的值为空。
树节点包含以下部件。
1. 数据。
2. 指向左子对象的指针。
3. 指向右子对象的指针。

在C中，我们可以使用结构来表示树节点。下面是具有整数数据的树节点的示例。

```C
struct node  
{ 
  int data; 
  struct node *left; 
  struct node *right; 
};
```

**C语言中的第一个简单树**。
让我们创建一个简单的树，在C中有4个节点。创建的树如下所示。

        tree
        ----
          1    <-- root
        /   \
       2     3  
      /   
     4

**代码实现**

```C
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
```

**总结**：树是一种分层数据结构。树的主要用途包括维护分层数据，提供适度的访问和插入/删除操作。二叉树是树的特例，其中每个节点至多有两个子节点。

下面是这个帖子的集合2和集合3
* [二叉树的性质](https://link) 
* [二叉树的类型](https://link) 
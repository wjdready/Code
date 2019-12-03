#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct avlTreeNode {
    int key;
    struct avlTreeNode *left;
    struct avlTreeNode *right;
    int height;
} avlTreeNode;


// 新建一个二叉树节点
avlTreeNode *newNode(int key)
{
    avlTreeNode *node = malloc(sizeof(avlTreeNode));

    node->height = 1;
    node->key = key;
    node->left = NULL;
    node->right = NULL;

    return node;
}

// 辅助函数，返回最大值
int max(int a, int b) 
{ 
    return (a > b) ? a : b; 
} 


// 获取二叉树的高度
int height(avlTreeNode *node) 
{ 
    if (node == NULL) 
        return 0; 
    return node->height; 
} 


// 获取节点 node 的平衡因子, 即 node 下的左右子树的高度差
int getBalance(avlTreeNode *node)
{
    if(node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

/*
                y                               x
               / \     Right Rotation          /  \
              x   T3   – - – - – - – >        T1   y
             / \       < - - - - - - -            / \
            T1  T2     Left Rotation            T2  T3
*/

// 向右旋转以 y 为根的树
avlTreeNode *rightRotate(avlTreeNode *y) 
{
    avlTreeNode *x = y->left;
    avlTreeNode *T2 = x->right;

    // 进行旋转
    x->right = y;  
    y->left = T2; 

    // 更新高度
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}


/*
                y                               x
               / \     Right Rotation          /  \
              x   T3   – - – - – - – >        T1   y
             / \       < - - - - - - -            / \
            T1  T2     Left Rotation            T2  T3
*/

// 向右旋转以 y 为根的树
avlTreeNode *leftRotate(avlTreeNode *x) 
{
    avlTreeNode *y = x->right;
    avlTreeNode *T2 = y->left;

    // 进行旋转
    y->left = x;  
    x->right = T2; 

    // 更新高度
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// 给定非空的二叉搜索树，
// 返回在该树中找到的具有最小键值的节点。
// 请注意，不需要搜索整个树
avlTreeNode * minValueNode(avlTreeNode* node) 
{
    avlTreeNode *currrnt = node;

    while(currrnt->left != NULL)
        currrnt = currrnt->left;
    return currrnt;
}

// 向AVL二叉树插入一个节点
avlTreeNode *avlTreeInsert(avlTreeNode *root, int key)
{
    // 1、执行正常的BST插入操作
    if(root == NULL)
        return newNode(key);
    // 如果键值已经存在
    if(key == root->key)
        return root;
    if(key < root->key)     //小于往左分支插
        root->left = avlTreeInsert(root->left, key);
    else
        root->right = avlTreeInsert(root->right, key);

    // 2、更新BST的高度, 即左右节点高度的最大值 + 1
    root->height = max(height(root->left), height(root->right)) + 1;
    
    // 3、获取该根节点的平衡因子，检查该节点是否变得不平衡
    int balance = getBalance(root); 

    // 如果此节点变得不平衡，则有4种情况, 这是由于插入导致的

    // 左 左 过长的原因
    if(balance > 1 && key < root->left->key) 
        return rightRotate(root);  

    // 右 右 过长原因
    if(balance < -1 && key > root->right->key) 
        return leftRotate(root);
    
    // 左 右 过长
        // Left Right Case  
    if(balance > 1 && key > root->left->key) {  
        root->left = leftRotate(root->left);  
        return rightRotate(root);  
    }  
  
    // 右 左 过长 
    if(balance < -1 && key < root->right->key) {  
        root->right = rightRotate(root->right);  
        return leftRotate(root);  
    }  
  
    // 若没做任何改变，返回原值
    return root;  
}


avlTreeNode *avlTreeDelete(avlTreeNode *root, int key)
{
    // 1、基本查找二叉树的删除
    if(root == NULL)
        return root;
    // 小于当前节点，则向左边查找删除
    if(key < root->key)
        root->left = avlTreeDelete(root->left, key);
    // 大于当前节点，则向右边查找删除
    else if (key > root->key) {
        root->right = avlTreeDelete(root->right, key);
    // 如果要删除的是当前 root 节点
    } else {
        // 当前根节点只有一个字节点或者没有子节点
        if(root->left == NULL || root->right == NULL) {
            avlTreeNode *temp = root->left ? 
                root->left : root->right;
            
            // 没有子节点情况, 直接删除该节点
            if(temp == NULL) {
                temp = root;
                root = NULL;
            // 有一个子节点情况，将字节点复制给根
            } else {
                *root = *temp;
            }
            free(temp);
        // 当前根节点包含两个子节点
        } else {
            // 获取右子树的后续节点中的最小值
            // 因为删除根节点后，右子树的最小值刚好适合做根节点的值
            avlTreeNode *temp =  minValueNode(root->right);

            // 复制该后续节点值给当前要删除的根节点
            root->key = temp->key;

            // 删除该后续最小值节点
            root->right = avlTreeDelete(root->right, temp->key);
        }
    }

    // 删除完后，如果树为空
    if(root == NULL)
        return root;
    
    // 2、更新当前节点的高度
    root->height = max(height(root->left), height(root->right)) + 1;

    // 3、获取平衡因子
    int balance = getBalance(root);

    // 如果此节点变得不平衡，则有4种情况, 这是由于删除导致的

	// Left Left Case 
	if (balance > 1 && getBalance(root->left) >= 0) 
		return rightRotate(root); 

	// Left Right Case 
	if (balance > 1 && getBalance(root->left) < 0) { 
		root->left = leftRotate(root->left); 
		return rightRotate(root); 
	} 

	// Right Right Case 
	if (balance < -1 && getBalance(root->right) <= 0) 
		return leftRotate(root); 

	// Right Left Case 
	if (balance < -1 && getBalance(root->right) > 0) { 
		root->right = rightRotate(root->right); 
		return leftRotate(root); 
	}

    return root;
}


void preOrder(avlTreeNode *root)  
{  
    if(root != NULL)  
    {  
        preOrder(root->left);  
        printf("%d ",root->key);
        preOrder(root->right);  
    }  
}  



typedef struct Trunk
{
    struct Trunk *prev;
    char *str;
}Trunk;


int main()
{
    void printTree(avlTreeNode *root, Trunk *prev, bool isLeft);


    avlTreeNode *root = NULL;
    typedef struct Trunk Trunk;

    root = avlTreeInsert(root, 9);  
    root = avlTreeInsert(root, 5);  
    root = avlTreeInsert(root, 10);  
    root = avlTreeInsert(root, 0);  
    root = avlTreeInsert(root, 6);  
    root = avlTreeInsert(root, 11);  
    root = avlTreeInsert(root, -1);  
    root = avlTreeInsert(root, 1);  
    root = avlTreeInsert(root, 3);
    root = avlTreeInsert(root, 4);

    root = avlTreeDelete(root, 10); 

    preOrder(root);
    printf("\n");

    printTree(root, NULL, false);

}



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
void printTree(avlTreeNode *root, Trunk *prev, bool isLeft)
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
    printf("%d\n",root->key);
    if (prev)
        prev->str = prev_str;
    trunk->str = "   |";

    printTree(root->right, trunk, false);
}
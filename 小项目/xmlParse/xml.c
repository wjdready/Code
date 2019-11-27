#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct xmltreeNode {
    struct xmltreeNode *next;
    struct xmltreeNode *prev;
    struct xmltreeNode *parent;
    struct xmltreeNode *child;
    struct xmltreeNode *lastChild;
    char *nodeName;
    char *nodeValue;
    char *attrName;     // 属性名
    char *attrValue;    // 属性值
    void *userData;     // 指向用户数据
} xmltreeNode;


xmltreeNode *xmlNewNode()
{
    xmltreeNode *node = malloc(sizeof(xmltreeNode));
    node->parent = NULL;
    node->child = NULL;
    node->lastChild = NULL;
    node->next = NULL;
    node->prev = NULL;
    node->nodeValue = NULL;
    node->nodeName = NULL;
    node->attrName = NULL;
    node->attrValue = NULL;
    node->userData = NULL;
}



xmltreeNode *mxmlLoadFile(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    int stack[100];
    int top = -1;

    int lines = 1;

    char nameBuffer[60];
    int nameBufIndex = 0;
    char valueBuffer[500];
    int valueBufIndex = 0;

    char attrNameBuffer[100];
    int attrNameIndex = 0;
    char attrValueBuffer[100];
    int attrValueIndex = 0;

    xmltreeNode *curNode = xmlNewNode();

    char ch;
    while((ch = fgetc(fp)) != EOF) {
        
        switch (ch)
        {
        case '<':
            stack[++top] = ch;
            break;

        case '>':
            nameBuffer[nameBufIndex++] = '\0';
            // 如果栈顶为 '<'，新建节点，并赋予属性值
            // 然后插入到在当前节点下，并把当前节点设置为新的节点
            if(stack[top] == '<') {
                // 新建节点
                xmltreeNode *node = xmlNewNode();
                node->nodeName = strdup(nameBuffer);
                // 插入到当前节点成为当前节点的孩子
                if(curNode->child == NULL) {
                    curNode->child = node;
                    curNode->lastChild = node;
                } else {
                    node->next = curNode->child;
                    curNode->child->prev = node;
                    curNode->child = node;
                }
                node->parent = curNode;
                curNode = node;

                // 清空节点名字的字符缓冲
                nameBufIndex = 0;

                // 保存属性值
                if(attrNameIndex != 0) {
                    curNode->attrName = strdup(attrNameBuffer);
                    attrNameIndex = 0;
                }
                if(attrValueIndex != 0) {
                    curNode->attrValue = strdup(attrValueBuffer);
                    attrValueIndex = 0;
                }

                // '<' 入栈
                stack[++top] = ch;

            // 如果栈顶为'/'，说明当前节点结束，保存节点值然后返回当前节点给父节点
            } else if(stack[top] == '/') {

                valueBuffer[valueBufIndex++] = '\0';
    
                // 节点名不匹配,打印错误
                if(strcmp(curNode->nodeName, nameBuffer) != 0) 
                    printf("error: 第%d行 节点名 %s 和 %s 不匹配\n",lines, curNode->nodeName, nameBuffer);

                // 保存节点值
                curNode->nodeValue = strdup(valueBuffer);
                valueBufIndex = 0;
                nameBufIndex = 0;

                // 父节点作为当前节点
                curNode = curNode->parent;

                // 出栈4次
                top -= 4;
            }
            break;

        case '\n':
            lines++;
            break;
        case '\t':
            break;
        case '\r':
            break;

        // / 入栈
        case '/':
            if(stack[top] == '<')
                stack[++top] = ch;
            break;
        
        // 如果是空格，且前面已经有节点名，则说明是元素属性, 空格入栈
        case ' ':
            if(stack[top] == '<' && nameBufIndex != 0) {
                stack[++top] = ch;
            }
            break;
        // 如果是等于号且栈顶是空格，说明属性值，直接获取属性值
        case '=':
            if(stack[top] == ' ') {
                while((ch = fgetc(fp)) != '\"')
                    ;
                // 获取属性值
                while((ch = fgetc(fp)) != '\"')  {
                    attrValueBuffer[attrValueIndex++] = ch;
                }
                attrValueBuffer[attrValueIndex] = '\0';
                attrNameBuffer[attrNameIndex] = '\0';
                // 将空格出栈
                top--;
            }
            break;

        default:
            // 其他正常字符
            if(stack[top] == '<' || stack[top] == '/')
                nameBuffer[nameBufIndex++] = ch;
            else if(stack[top] == '>')
                valueBuffer[valueBufIndex++] = ch;
            // 栈顶是空格，说明当前累加的是属性名
            else if (stack[top] == ' ') {
                attrNameBuffer[attrNameIndex++] = ch;
            }

            break;
        } 
    }
    // 返回根节点
    xmltreeNode *retRoot = curNode->child;
    free(curNode);
    fclose(fp);

    return retRoot;
}

void printXmlNodeNoend(xmltreeNode *node)
{
    if(node != NULL) {
        if(node->nodeName != NULL)
            printf("<%s",node->nodeName);
        if(node->attrName != NULL)
            printf(" %s=",node->attrName);
        if(node->attrValue != NULL)
            printf("\"%s\"",node->attrValue);
        printf(">");
        if(node->nodeValue != NULL)
            printf("%s", node->nodeValue);
    }
}

void printTab(int top)
{
    while(top >= 0) {
        printf("  ");
        top--;
    }
}

void printEnd(xmltreeNode *root, int tab)
{
    if(root->lastChild != NULL)
        printTab(tab);
    printf("</%s>\n",root->nodeName);
}


void printXmlTree(xmltreeNode *root, int *stack, int top)
{
    if(root != NULL) {

        // 先打印该节点，没有结尾
        printXmlNodeNoend(root);

        //保存同级节点的tab数
        int tabTmp = top; 

        // 如果该节点不是叶子节点，则换行
        if(root->lastChild != NULL) {
            printf("\n");
        }

        // 再打印该节点的最后一个子节点
        if(root->lastChild != NULL) {
            top++;
            printTab(top);
            printXmlTree(root->lastChild, stack, top);
        }
        
        // 然后向左边打印同级节点
        if(root->prev != NULL) {
            // 先打印前一个结点的结尾
            printEnd(root, tabTmp);
            printTab(tabTmp);
            printXmlTree(root->prev, stack, tabTmp);
        }

        // 同级节点结尾打印过了，直接返回
        if(root->prev != NULL) {
            return;
        }

        // 打印结尾
        printEnd(root, tabTmp);
       
    }
}

void printXmlTreeCall(xmltreeNode *root)
{
    int stack[100];
    int top = -1;
    printXmlTree(root, stack, top);
}



void XmlToUserData(xmltreeNode *root)
{
    
}


int main()
{
    xmltreeNode *root = mxmlLoadFile("gg.xml");
    
    printXmlTreeCall(root);
    return 0;
}
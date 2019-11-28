#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xml.h"


/**
 * 新建一个xml节点
 * 
 * 注意：初始情况下所有都指向空，所以需要定义好后自己赋值
 * 因为后续要释放该节点，所以给指针赋值的时候，指向的地方
 * 必须是堆，不能是常量空间或者函数栈。
 * 
 * 例如对于字符串的赋值，可以用 
 * node->nodeName = strdup("string");
 */
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


/**
 * 加载xml文件并解析成xml树
 * 
 * 示例：
 * xmltreeNode *root = mxmlLoadFile("file.xml");
 * 这时候我们便拿到一个xml树的根，可以通过该root对xml树进行操作
 */ 
xmltreeNode *mxmlLoadFile(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("error: 打开文件失败\n");
        return NULL;
    }
        
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



/**
 * 打印的辅助函数, 非外部调用
 * 
 * 按格式打印xml树的指定节点，不包含结尾</>
 * 
 * 例如:
 *      打印 <node>good</node>
 *      则输出 <node>good
 *      打印 <node attr="big">good</node>
 *      则输出 <node attr="big">good
 */ 
static void printXmlNodeNoEnd(xmltreeNode *node, FILE *fp)
{
    if(node != NULL) {
        if(node->nodeName != NULL)
            fprintf(fp, "<%s",node->nodeName);
        if(node->attrName != NULL)
            fprintf(fp, " %s=",node->attrName);
        if(node->attrValue != NULL)
            fprintf(fp, "\"%s\"",node->attrValue);
        fprintf(fp, ">");
        if(node->nodeValue != NULL)
            fprintf(fp, "%s", node->nodeValue);
    }
}


/**
 * 打印的辅助函数, 非外部调用
 * 
 * 按格式打印tab
 * 
 * 将根据tab栈的大小打印tab的数量
 */ 
static void printTab(int top, FILE *fp)
{
    while(top >= 0) {
        fprintf(fp, "  ");
        top--;
    }
}

/**
 * 打印的辅助函数, 非外部调用,
 * 
 * 打印结点尾部
 * 
 * 由于printXmlNodeNoEnd()没有打印结尾，故需要调用此函数进行关闭节点
 */ 
static void printEnd(xmltreeNode *root, int tab, FILE *fp)
{
    if(root->lastChild != NULL)
        printTab(tab, fp);
    fprintf(fp, "</%s>\n",root->nodeName);
}


/**
 * 打印的辅助函数, 非外部调用
 * 
 * 根据FILE参数，判断输出打印到文件中还是标准输出中
 * 或者是其他类型的 _FILE_IO_
 */ 
static void printXmlTreeHleper(xmltreeNode *root, int *stack, int top, FILE *fp)
{
    if(root != NULL) {

        // 先打印该节点，没有结尾
        printXmlNodeNoEnd(root, fp);

        //保存同级节点的tab数
        int tabTmp = top; 

        // 如果该节点不是叶子节点，则换行
        if(root->lastChild != NULL) {
            fprintf(fp, "\n");
        }

        // 再打印该节点的最后一个子节点
        if(root->lastChild != NULL) {
            top++;
            printTab(top, fp);
            printXmlTreeHleper(root->lastChild, stack, top, fp);
        }
        
        // 然后向左边打印同级节点
        if(root->prev != NULL) {
            // 先打印前一个结点的结尾
            printEnd(root, tabTmp, fp);
            printTab(tabTmp, fp);
            printXmlTreeHleper(root->prev, stack, tabTmp, fp);
        }

        // 同级节点结尾打印过了，直接返回
        if(root->prev != NULL) {
            return;
        }

        // 打印结尾
        printEnd(root, tabTmp, fp);
       
    }
}


/**
 * 将内存中的xml树保存到指定的文件中
 * 
 * root 是传入的xml树的根节点， filename是要保存的文件名
 * 
 * 如果保存失败，则不做任何操作，直接返回
 */
void saveXmlTreeToFile(xmltreeNode *root, const char *filename)
{
    int stack[100];
    int top = -1;
    FILE *fp = fopen(filename, "w");
    if(fp == NULL) {
        printf("error: 保存时打开文件出错\n");
        return;
    }
    printXmlTreeHleper(root, stack, top, fp);
    fclose(fp);
}


/**
 * 打印xml树到标准输出
 * 
 * root xml树的根
 */
void printXmlTree(xmltreeNode *root)
{
    int stack[100];
    int top = -1;
    
    printXmlTreeHleper(root, stack, top, stdout);
}


/**
 * 添加xml节点到某个父节点下
 * 
 * parent 要添加到的父节点
 * node 添加的节点
 * 
 * 例如：
 *      将child节点添加到parent下
 *      xmlAddNode(parent, child);
 */ 
void xmlAddNode(xmltreeNode *parent, xmltreeNode *node) 
{
    // 如果父节点为空，则直接添加
    if(parent->child == NULL) {
        parent->child = parent->lastChild = node;
    } else {
    // 否则插从头插入
        node->next = parent->child;
        node->prev = NULL;
        parent->child->prev = node;
        parent->child = node;  
    }
    node->parent = parent;
}


/**
 * 释放xml的一个叶子节点及其节点内指针指向的堆内存
 * 
 * node: 指向xml树的一个结点，该节点必须是叶子节点
 */ 
static void freeXmlNode(xmltreeNode *node)
{
    if(node->attrName != NULL) {
        free(node->attrName);
        printf("free\n");
    }
        
    if(node->attrValue != NULL) {
        free(node->attrValue);
        printf("free\n");
    }
        
    if(node->nodeName != NULL) {
        free(node->nodeName);
        printf("free\n");
    }
        
    if(node->nodeValue != NULL) {
        free(node->nodeValue);
        printf("free\n");
    }
}


/**
 * 删除xml节点及其节点下的所有子节点
 * 
 * node: 如果node是根节点(即parent为空)，则不做任何事情
 */ 
void xmlDeleteNode(xmltreeNode *node)
{
    // 如果父节点为空，则说明是根，直接返回
    if(node->parent == NULL) 
        return;
     
    xmltreeNode *parent = node->parent;

    // 删除其所有孩子
    xmltreeNode *child = node->child;
    xmltreeNode *p;
    while (child != NULL) {
        p = child;
        child = child->next;
        xmlDeleteNode(p);
    }

    // 如果同级下有两个以上的节点
    if(parent->lastChild != parent->child) {
       
        // 如果该节点是第一个孩子
        if(node->prev == NULL) {
            parent->child = node->next;
            node->next->prev = NULL;
        // 如果该节点是最后一个孩子
        } else if(node->next == NULL) {
            parent->lastChild = node->prev;
            node->prev->next = NULL;
        // 中间节点
        } else {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
    // 否则只有一个节点，则父节点直接指向空
    } else {
        parent->child = parent->lastChild = NULL;
    }

    freeXmlNode(node);
}


/**
 * 删除xml树本身以及树下所有子节点
 * 
 * root xml树的树根，且必须是树根，否则删除后其父节点将
 * 无法找到该节点。
 */ 
void xmlTreeDelete(xmltreeNode *root)
{
    // 删除其所有孩子
    xmltreeNode *child = root->child;
    xmltreeNode *p;
    while (child != NULL) {
        p = child;
        child = child->next;
        xmlDeleteNode(p);
    }
    // 删除根节点
    freeXmlNode(root);
}
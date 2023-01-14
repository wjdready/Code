#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "newxml.h"

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
    // 属性值链表初始化
    node->attrList.head = NULL;
    node->attrList.tail = NULL;
    node->attrList.len = 0;
    node->userData = NULL;
}

/**
 * 新建属性节点
 */ 
attrNode *newAttrNode()
{
    attrNode *node = (attrNode *)malloc(sizeof(attrNode));
    node->attrName = NULL;
    node->attrValue = NULL;
    node->next = NULL;
    node->prev = NULL;
    return node;
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
 * 为节点添加属性值
 * node 要添加到的节点
 * 
 * attr属性节点指针
 */ 
void xmlAddAttr(xmltreeNode *node, attrNode *attr)
{
    // 将属性值插入到节点 
    if(node->attrList.len == 0) {
        node->attrList.head = attr;
        node->attrList.tail = node->attrList.head;
    } else {
        attr->next = node->attrList.head;
        node->attrList.head->prev = attr;
        node->attrList.head = attr;
    }
    node->attrList.len++;
}

/**
 * 忽略空格和回车符号
 * 
 * 辅助函数，非外部调用
 */ 
static int ignoreSpaceEnter(FILE *fp, int *lines)
{
    int ch;
    while((ch  = fgetc(fp)) == ' ' || ch == '\n') {
        if(ch == '\n')
            (*lines)++;
    }
    return ch;
}

/**
 * 忽略空格符号
 * 
 * 辅助函数，非外部调用
 */ 
static int ignoreSpace(FILE *fp)
{
    int ch;
    while((ch  = fgetc(fp)) == ' ')
        ;
    return ch;
}


/**
 * 解析节点名
 * 
 * 辅助函数，非外部调用
 */ 
static int nodeNameParse(xmltreeNode *node, FILE *fp, int ch, int *lines)
{
    int len = 0;
    char buf[100];
    buf[len++] = ch;
    while((ch = fgetc(fp)) != ' ' && ch != '>') {
        if(ispunct(ch)) {
            printf("error: 第%d行, 节点名中包含非法字符 '%c' \n", *lines, ch);
            return -1;
        }
        buf[len++] = ch;
    }
    buf[len] = '\0';

    node->nodeName = strdup(buf);
    
    return ch;
}

/**
 * 解析节点属性
 * 
 * 辅助函数，非外部调用
 */
static int attrParse(xmltreeNode *node, FILE *fp, int ch, int *lines)
{
    int len = 0;
    char buf[100];

    // 新建一个属性值
    attrNode *attr = newAttrNode();

    // 忽略空格和换行
    ch = ignoreSpaceEnter(fp, lines);
    if(ch == '>')
        return ch;

    // 保存属性名
    buf[len++] = ch;
    while((ch  = fgetc(fp)) != '=' && ch != ' ') {
        if(ispunct(ch)) {
            buf[len] = '\0';
            printf("error: 第%d行，属性名'%s%c'非法\n",*lines, buf, ch);
            return -1;
        }
        buf[len++] = ch;
    }
    buf[len] = '\0';
    attr->attrName = strdup(buf);

    // 保存属性值
    len = 0;
    // 忽略空格和换行
    ch = ignoreSpaceEnter(fp, lines);
    // 如果是 name   ="value" 的情况
    if(ch == '=') {
        ch = ignoreSpaceEnter(fp, lines);
    }
    if(ch != '\"') {
        printf("error: 第%d行，'%s' 后存在非法字符\n", *lines, buf);
        return -1;
    }
    while ((ch  = fgetc(fp)) != '\"') {
        buf[len++] = ch;
    }
    buf[len] = '\0';
    attr->attrValue = strdup(buf);
    xmlAddAttr(node, attr);
    // 正常读取一个属性值，ch返回值应该是 "
    return ch;
}


/**
 * 加载xml文件并解析成xml树
 * 
 * 示例：
 * xmltreeNode *root = mxmlLoadFile("file.xml");
 * 这时候我们便拿到一个xml树的根，可以通过该root对xml树进行操作
 */ 
xmltreeNode *laodFileXml(const char *filename) 
{
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("error: 打开文件失败\n");
        return NULL;
    }

    xmltreeNode *curNode = xmlNewNode();
    int lines = 1;

    int len = 0;
    char buf[500];

    int ch;
    while((ch = fgetc(fp)) != EOF) {
        if(ch == '<') {
            ch = fgetc(fp);
            // 检测 < 后面的字符是否非法，即除了 ! / 外不能是标点符号或空格
            if(ispunct(ch) && ch != '/' && ch != '!' || isspace(ch)) {
                printf("error: 第%d行, '<'后面非法字符 '%c' \n", lines, ch);
                return NULL;
            }
            // 如果是节点结束符
            if(ch == '/') {
                // 保存节点值
                buf[len] = '\0';
                curNode->nodeValue = strdup(buf);
                len = 0;

                while((ch = fgetc(fp)) != '>') {
                    if(ispunct(ch)) {
                        buf[len] = '\0';
                        printf("error: 第%d行，节点名存在非法字符'%s%c'\n", lines, buf, ch);
                        return NULL;
                    }
                    buf[len++] = ch;
                }
                buf[len] = '\0';
                if(strcmp(curNode->nodeName, buf)) {
                    printf("error: 第%d行，节点名头 '%s' 和尾 '%s' 不匹配\n", lines, curNode->nodeName,buf);
                    return NULL;
                }
                len = 0;
                buf[len] = '\0';
                // 获取一个完整节点则返回当前节点给父节点
                curNode = curNode->parent;
            // 如果是注释
            } else if(ch == '!') {
            
            // 是节点名
            } else {
                // 新建节点，并解析出节点名
                xmltreeNode *node = xmlNewNode();
                ch = nodeNameParse(node, fp, ch, &lines);
                if(ch == -1) {
                    return NULL;
                }
                // 解析属性值
                if(ch == ' ') {
                   while((ch = attrParse(node, fp, ch, &lines)) != '>') {
                       if(ch == -1)
                            return NULL;
                   }
                }
                // 将新结点添加到当前节点下，并更新当前节点
                xmlAddNode(curNode, node);
                curNode = node;
            }
            // > 结束， 重新累加buf
            len = 0;
        // 结束 < >
        } else if(ch == '\n') {
            lines++;
        } else {
            // 保存节点值
            buf[len++] = ch;
        }
    }
    fclose(fp);
    return curNode->child;
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
static void printXmlNodeNoEnd(xmltreeNode *node, FILE *fp, int withTokens)
{
    if(node != NULL) {
        if(node->nodeName != NULL) {
            if(withTokens) 
                fprintf(fp, "<%s",node->nodeName);
            else 
                fprintf(fp, "%s",node->nodeName);
        }
            
        // 打印属性值
        if(node->attrList.len > 0 ) {
            attrNode *attr = node->attrList.tail;
            attrNode *p;
            while(attr) {
                p = attr;
                attr = attr->prev;
                if(withTokens) {
                    fprintf(fp, " %s=",p->attrName);
                    fprintf(fp, "\"%s\"",p->attrValue);
                } else {
                    fprintf(fp, " %s=",p->attrName);
                    fprintf(fp, "%s",p->attrValue);
                }
            }
        }
        if(withTokens) 
            fprintf(fp, ">");
        else 
            fprintf(fp, " ");

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
static void printTab(int tab, FILE *fp)
{
    while(tab > 0) {
        fprintf(fp, "  ");
        tab--;
    }
}

/**
 * 打印的辅助函数, 非外部调用,
 * 
 * 打印结点尾部
 * 
 * 由于printXmlNodeNoEnd()没有打印结尾，故需要调用此函数进行关闭节点
 */ 
static void printEnd(xmltreeNode *root, int tab, FILE *fp,  int withTokens)
{
    if(root->lastChild != NULL)
        printTab(tab, fp);
    if(withTokens)
        fprintf(fp, "</%s>\n",root->nodeName);
    else {
        if(root->prev != NULL)
            fprintf(fp, "\n");
    }
}


/**
 * 打印的辅助函数, 非外部调用
 * 
 * tab 表示当前节点打印tab的数量
 * 根据FILE参数，判断输出打印到文件中还是标准输出中
 * 或者是其他类型的 _FILE_IO_
 */ 
static void printXmlTreeHleper(xmltreeNode *root, int tab, FILE *fp, int withTokens)
{
   
    if(root == NULL) 
        return ;
    
    // 先打印该节点，没有结尾
    printTab(tab, fp);
    printXmlNodeNoEnd(root, fp, withTokens);

     // 如果该节点不是叶子节点，则换行
    if(root->lastChild != NULL) {
        fprintf(fp, "\n");
    }

    xmltreeNode *child = root->lastChild;

    // 再打印其所有孩子
    while(child != NULL) {
        printTab(tab+1, fp);
        printXmlTreeHleper(child, tab+1, fp, withTokens);
        child = child->prev;
    }
    // 打印结尾
    if(root->lastChild != NULL) {
        // 非叶子节点打印结尾前打tab
        printTab(tab, fp);
    }

    printEnd(root, tab, fp, withTokens);
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
    FILE *fp = fopen(filename, "w");
    if(fp == NULL) {
        printf("error: 保存时打开文件出错\n");
        return;
    }
    printXmlTreeHleper(root, 0, fp, 1);
    fclose(fp);
}


/**
 * 打印xml树到标准输出
 * 
 * root xml树的根
 * 
 * withTokens 1 打印xml符号 0 不打印xml符号
 * 
 */
void printXmlTree(xmltreeNode *root, int withTokens)
{
    printXmlTreeHleper(root, 0, stdout, withTokens);
    if(!withTokens)
        printf("\n");
}


/**
 * 释放单个 attr 属性节点
 */ 
static void freeAttrlNode(attrNode *node)
{
    if(node->attrName != NULL) {
        // printf("free1\n");
        free(node->attrName);
    }
    if(node->attrValue != NULL) {
        // printf("free2\n");
        free(node->attrValue);
    }
    // printf("free3\n");
    free(node);
}


/**
 * 释放xml的一个叶子节点及其节点内指针指向的堆内存
 * 
 * node: 指向xml树的一个结点，该节点必须是叶子节点
 */ 
static void freeXmlNode(xmltreeNode *node)
{
    // 释放属性值链表
    if(node->attrList.len > 0) {
        attrNode *current = node->attrList.head;
        attrNode *tmp;
        while (current != NULL) {
            tmp = current;
            current = current->next;
            freeAttrlNode(tmp);
        }
        node->attrList.len = 0;
    }

    if(node->nodeName != NULL) {
        // printf("free4\n");
        free(node->nodeName);
    }
        
    if(node->nodeValue != NULL) {
        // printf("free5\n");
        free(node->nodeValue);
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


/**
 * 给定节点，通过属性名查找属性
 * 
 * node 要查找的结点
 * name 要查找该节点下的属性的属性名
 * 
 * 如果属性值则返回该属性值，否则返回NULL
 */ 
attrNode *searcAttrList(xmltreeNode *node, const char *name)
{
    if(node ==  NULL)
        return NULL;
    attrNode *attr = node->attrList.head;
    while (attr != NULL) {
        if(!strcmp(attr->attrName, name))
            return attr;
        attr = attr->next;
    }
    return NULL;
}

/**
 * 查找父节点下，具有某个属性的孩子
 * 
 * attrName 要查找的孩子具有的属性名
 * arrtValue 要查找的孩子具有的属性值
 * 
 * 如果找到，则返回该孩子，否则返回空
 */ 
xmltreeNode *searchChildWithAttr(xmltreeNode *parent, const char *attrName, const char *attrValue)
{
    xmltreeNode *child = parent->child;
    attrNode *attr;
    while(child != NULL) {
        // 查找当前孩子是否具有属性 attrName
        attr = searcAttrList(child, attrName);
        if(attr != NULL)
            if(!strcmp(attr->attrValue, attrValue))
                return child;
        child = child->next;
    }
    return NULL;
}


/**
 * 查找父节点下具有某个结点名的孩子
 * 
 * parent 要查找的父节点
 * nodeName 孩子结点名
 * 
 * 如果找到，则返回孩子，否则返回NULL
 */ 
xmltreeNode *searchChild(xmltreeNode *parent, const char *nodeName)
{
    xmltreeNode *child = parent->child;

    while (child != NULL) {
        if(!(strcmp(child->nodeName, nodeName)))
        return child;
    }
    return NULL;
    
}
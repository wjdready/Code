#ifndef __XML_H__
#define __XML_H__

typedef struct xmltreeNode {
    struct xmltreeNode *next;   // 指向同级节点(即兄弟节点)的下一个节点
    struct xmltreeNode *prev;   // 指向同级节点(即兄弟节点)的前一个节点
    struct xmltreeNode *parent; // 指向父节点
    struct xmltreeNode *child;  // 指向第一个子节点
    struct xmltreeNode *lastChild;  // 指向最后一个子节点
    char *nodeName;     // 节点名
    char *nodeValue;    // 节点值
    char *attrName;     // 属性名
    char *attrValue;    // 属性值
    void *userData;     // 指向用户数据
} xmltreeNode;

// 新建一个xml节点
xmltreeNode *xmlNewNode();

// 加载xml文件并解析成xml树
xmltreeNode *mxmlLoadFile(const char *filename);

// 将内存中的xml树保存到指定的文件中
void saveXmlTreeToFile(xmltreeNode *root, const char *filename);

// 打印xml树到标准输出
void printXmlTree(xmltreeNode *root);

// 添加xml节点到某个父节点下
void xmlAddNode(xmltreeNode *parent, xmltreeNode *node);

// 删除xml节点及其节点下的所有子节点
void xmlDeleteNode(xmltreeNode *node);

// 删除xml树本身以及树下所有子节点
void xmlTreeDelete(xmltreeNode *root);

#endif
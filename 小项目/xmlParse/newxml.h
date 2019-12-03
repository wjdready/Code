#ifndef __NEWXML_H__
#define __NEWXML_H__


typedef struct attrNode {
    char *attrName;
    char *attrValue;
    struct attrNode *next;
    struct attrNode *prev;
} attrNode;

typedef struct attrList
{
    struct attrNode *head;
    struct attrNode *tail;
    unsigned long len;
} attrList;

typedef struct xmltreeNode {
    struct xmltreeNode *next;   // 指向同级节点(即兄弟节点)的下一个节点
    struct xmltreeNode *prev;   // 指向同级节点(即兄弟节点)的前一个节点
    struct xmltreeNode *parent; // 指向父节点
    struct xmltreeNode *child;  // 指向第一个子节点
    struct xmltreeNode *lastChild;  // 指向最后一个子节点
    char *nodeName;     // 节点名
    char *nodeValue;    // 节点值
    attrList attrList;
    void *userData;     // 指向用户数据
} xmltreeNode;

// 新建一个xml节点
xmltreeNode *xmlNewNode();

// 新建属性节点
attrNode *newAttrNode();


// 添加xml节点到某个父节点下
void xmlAddNode(xmltreeNode *parent, xmltreeNode *node);

// 为节点添加属性值
void xmlAddAttr(xmltreeNode *node, attrNode *attr);

// 加载xml文件并解析成xml树
xmltreeNode *laodFileXml(const char *filename);

// 将内存中的xml树保存到指定的文件中
void saveXmlTreeToFile(xmltreeNode *root, const char *filename);

// 打印xml树到标准输出
void printXmlTree(xmltreeNode *root, int withTokens);

// 删除xml节点及其节点下的所有子节点
void xmlDeleteNode(xmltreeNode *node);

// 删除xml树本身以及树下所有子节点
void xmlTreeDelete(xmltreeNode *root);

// 给定节点，通过属性名查找结点下是否存在该属性
attrNode *searcAttrList(xmltreeNode *node, const char *name);

// 查找父节点下，具有某个属性的孩子
xmltreeNode *searchChildWithAttr(xmltreeNode *parent, const char *attrName, const char *attrValue);

xmltreeNode *searchChild(xmltreeNode *parent, const char *nodeName);


#endif
#include "xml.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


xmltreeNode *newXmlStuNode(const char *name, int age, int number)
{
    // 新建一个学生节点
    xmltreeNode *student = xmlNewNode();
    student->nodeName = strdup("student");
    student->attrName = strdup("name");
    student->attrValue = strdup(name);

    char str[30];
    // 往学生节点里添加年龄信息
    xmltreeNode *stuAge = xmlNewNode();
    stuAge->nodeName = strdup("age");
    snprintf(str, sizeof(str), "%d", age);
    stuAge->nodeValue = strdup(str);
    xmlAddNode(student, stuAge);

    // 往学生节点里添加学号信息
    xmltreeNode *stuNumber = xmlNewNode();
    stuNumber->nodeName = strdup("number");
    snprintf(str, sizeof(str), "%d", number);
    stuNumber->nodeValue = strdup(str);
    xmlAddNode(student, stuNumber);

    return student;
}

xmltreeNode *newXmlClassNode(const char *className) 
{
    xmltreeNode *class = xmlNewNode();
    class->nodeName = strdup("class");
    class->attrName = strdup("name");
    class->attrValue = strdup(className);
    
    return class;
}

 
int main()
{

    // 新建一个学校
    xmltreeNode *school = xmlNewNode();
    school->nodeName = strdup("school");

    // 新建班级和学生
    xmltreeNode *class = newXmlClassNode("1班");
    xmltreeNode *stu1 = newXmlStuNode("张泽", 23, 45);
    xmltreeNode *stu2 = newXmlStuNode("孙哲", 34, 56);
    
    // 把班级添加到学校
    xmlAddNode(school, class);

    // 把学生添加到班级
    xmlAddNode(class, stu1);
    xmlAddNode(class, stu2);

    // 打印学校结构
    printXmlTree(school);
    //保存到文件中
    saveXmlTreeToFile(school, "school.xml");
}
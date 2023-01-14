#include "newxml.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


// 新建属性
attrNode *newAttr(const char *name, const char *value)
{
    // 属性名和属性值不能为空
    if(name == NULL || value == NULL)
        return NULL;
    attrNode *attr = newAttrNode();
    attr->attrName = strdup(name);
    attr->attrValue = strdup(value);
    return attr;
}

// 
xmltreeNode *newNode(const char *name, const char *value)
{
    // 必须至少有节点名
    if(name == NULL)
        return NULL;
    xmltreeNode *node = xmlNewNode();
    node->nodeName = strdup(name);
    if(value != NULL)
        node->nodeValue = strdup(value);
    return node;
}



xmltreeNode *newXmlStuNode(const char *name, int age, int number, const char *email, const char *addr)
{
    char str[30];
    
    // 新建一个学生节点
    xmltreeNode *student = newNode("student", NULL);
    // 往学生里添加名字属性
    xmlAddAttr(student, newAttr("name", name));
    // 往学生里添加年龄属性
    snprintf(str, sizeof(str), "%d", age);
    xmlAddAttr(student, newAttr("age", str));
    // 往学生里添加学号属性
    snprintf(str, sizeof(str), "%d", number);
    xmlAddAttr(student, newAttr("number", str));

    // 往学生里添加邮箱信息
    xmlAddNode(student, newNode("email", email));

    // 往学生里添加地址信息
    xmlAddNode(student, newNode("addr", addr));

    return student;
}

xmltreeNode *newXmlClassNode(const char *className, int number) 
{
    char str[30];
    
    xmltreeNode *class = newNode("class", NULL);

    // 设置班级名
    xmlAddAttr(class, newAttr("name", className));

    // 设置班级人数
    snprintf(str, sizeof(str), "%d", number);
    xmlAddAttr(class, newAttr("number", str));
    
    return class;
}


/**
 * 在班级下以姓名为条件查找学生信息
 * 
 * 如果找到，则返回该学生
 * 
 * 如果找不到，则返回NULL
 */ 
xmltreeNode* searchStudentName(xmltreeNode *class, const char *studentName)
{
    return searchChildWithAttr(class, "name", studentName);
}


xmltreeNode* searchClassName(xmltreeNode *school, char *className)
{
    return searchChildWithAttr(school, "name", className);
}

xmltreeNode *newSchool(xmltreeNode *school)
{
    if(school != NULL) {
        printf("当前已经有学校，要覆盖当前学校吗？\n");
        return school;
    }
    char buf[100];
    school = xmlNewNode();
    printf("请输入学校名字：");
    scanf("%s",buf);
    school->nodeName = strdup(buf);

    return school;
}

void newClass(xmltreeNode *school)
{
    if(school == NULL) {
        printf("还没有学校，需要先新建学校, 或从文件中加载学校信息表\n");
        return ;
    }

    char className[100];
    int classNumber;

    printf("请输入班级名字：");
    scanf("%s",className);
    printf("请输入班级人数：");
    scanf("%d", &classNumber);
    xmlAddNode(school, newXmlClassNode(className, classNumber));
    // printXmlTree(school, 2);
}


void deleteClass(xmltreeNode *school)
{
    if(school == NULL) {
        printf("还没有学校，需要先新建学校, 或从文件中加载学校信息表\n");
        return ;
    }

    char className[100];
    printf("选择要删除的班级名\n");
    scanf("%s", className);

    xmltreeNode *class = searchClassName(school, className);
    if(class == NULL) {
        printf("没有找到班级\n");
        return ;
    }
    xmlDeleteNode(class);
    // printXmlTree(school, 0);
}


void newStudent(xmltreeNode *school)
{
    if(school == NULL) {
        printf("还没有学校，需要先新建学校, 或从文件中加载学校信息表\n");
        return ;
    }

    char className[100];
    printf("请输入学生班级：");
    scanf("%s", className);

    xmltreeNode *class = searchClassName(school, className);
    if(class == NULL) {
        printf("没有找到班级，你应该先添加这个班级再添加学生\n");
        return ;
    }
    char name[100];
    int age;
    int number;
    char email[100];
    char addr[100];
    printf("请输入学生姓名：");
    scanf("%s", name);
    printf("请输入学生年龄：");
    scanf("%d", &age);
    printf("请输入学生学号：");
    scanf("%d", &number);
    printf("请输入学生邮箱：");
    scanf("%s", email);
    printf("请输入学生地址：");
    scanf("%s", addr);
    xmlAddNode(class, newXmlStuNode(name, age, number, email, addr));
}   

void saveSchoolToFile(xmltreeNode *school)
{
    if(school == NULL) {
        printf("还没有学校，需要先新建学校, 或从文件中加载学校信息表\n");
        return ;
    }

    char filename[100];
    printf("输入要保存的文件名: ");
    scanf("%s", filename);

    strcat(filename, ".xml");
    saveXmlTreeToFile(school, filename);

}

void changeStuInfo(xmltreeNode *student)
{
    char buf[100];
    printf("更改学生的什么信息: ");

    scanf("%s", buf);
    attrNode *attr = searcAttrList(student, buf);
   
    if(attr != NULL) {
        printf("输入新的信息：");
        scanf("%s", buf);
        free(attr->attrValue);
        attr->attrValue = strdup(buf);
    // 找的不是属性值
    } else {
        xmltreeNode *info = searchChild(student, buf);
        if(info == NULL) {
            printf("没找到学生信息\n");
            return ;
        }
        printf("输入新的信息：");
        scanf("%s", buf);
        free(info->nodeValue);
        info->nodeValue = strdup(buf);
    }

}


void searchStudent(xmltreeNode *school)
{
    if(school == NULL) {
        printf("还没有学校，需要先新建学校, 或从文件中加载学校信息表\n");
        return ;
    }
    char name[100];
    printf("输入要查找学生所在的班级: ");
    scanf("%s", name);
    xmltreeNode *class = searchClassName(school, name);
    if(class == NULL) {
        printf("没有找到该班级\n");
        return ;
    }
    printf("输入要查找学生姓名: ");
    scanf("%s", name);
    xmltreeNode *student = searchStudentName(class, name);
    if(student == NULL) {
        printf("没有找到该学生\n");
        return ;
    }
    printf("找到学生，信息如下:\n");
    printXmlTree(student, 0);
    printf("\n1、删除该学生\n");
    printf("\n2、更改学生信息\n");
    printf("\n3、不做任何操作\n");
    printf("请问要对该学生做什么操作: ");

    int cmd = 0;
    scanf("%d", &cmd);
    if(cmd == 1) {
    // 删除该学生
        xmlDeleteNode(student);
    } else if(cmd == 2) {
        changeStuInfo(student);
    }
}

void exitPro(xmltreeNode *school, int saved)
{
     if(school != NULL && saved == 0) {
        printf("1、保存  2、直接退出\n");
        printf("还没保存，是否保存：");
        int save = 0;
        scanf("%d", &save);
        if(save == 1) 
            saveXmlTreeToFile(school, "school.xml");
    }
}

xmltreeNode *loadSchool(xmltreeNode *school)
{
    printf("请输入要加载的文件名字: ");
    char buf[100];
    scanf("%s", buf);
    school = laodFileXml(buf);
    if(school == NULL) 
        printf("找不到该文件\n");
    return school;
}

void displayHelp()
{
    printf("\033[36m----------------------------------------------------------\033[0m\n");
    printf("\033[36m-               欢迎使用xml解析学生信息系统              -\033[0m\n");
    printf("\033[36m----------------------------------------------------------\033[0m\n");
    printf("\033[36m-1.新建记录                                              -\033[0m\n");
    printf("\033[36m-2.增加班级                                              -\033[0m\n");//根据学校，class，学号插入；
    printf("\033[36m-3.删除班级                                              -\033[0m\n");
    printf("\033[36m-4.增加学生                                              -\033[0m\n");
    printf("\033[36m-5.查找学生                                              -\033[0m\n");
    printf("\033[36m-6.打印学校                                              -\033[0m\n");
    printf("\033[36m-7.保存到xml                                             -\033[0m\n");
    printf("\033[36m-8.从xml加载                                             -\033[0m\n");
    printf("\033[36m-9.退出程序                                              -\033[0m\n");
    printf("\033[36m----------------------------------------------------------\033[0m\n");
}



int judgeCmd()
{   
    xmltreeNode *school = laodFileXml("school.xml");
    int cmd = 0;
    int saved = 1;

    while(1)
    {
        displayHelp();
        
        scanf("%d" ,&cmd);
        system("clear");
        switch (cmd)
        {
        case 1:
            school = newSchool(school);
            saved = 0;
            break;
        case 2:
            newClass(school);
            break;
        case 3:
            deleteClass(school);
            saved = 0;
            break;
        case 4:
            newStudent(school);
            saved = 0;
            break;
        case 5:
            searchStudent(school);
            break;
        case 6:
            printXmlTree(school, 0);
            break;
        case 7:
            saveSchoolToFile(school);
            saved = 1;
            break;
        case 8:
            school = loadSchool(school);
            break;
        case 9:
            exitPro(school, saved);
            return 0;
        default:
            break;
        }
        // 忽略后面内容
        while((cmd = getchar()) != '\n')
            ;
    }
}


int main()
{
    judgeCmd();
    
}
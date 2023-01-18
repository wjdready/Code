#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "adlist.h"

// 定义学生类型
typedef struct {
    char name[20];
    int number;
    int age;
    float source;
} student;


// 创建一个学生
student *createStudent(const char name[], int number, 
            int age, float source)
{
    student *stu = (student *)malloc(sizeof(student));
    strcpy(stu->name, name);
    stu->number = number;
    stu->age = age;
    stu->source = source;

    return stu;
}

// 打印学生信息
void print_student(student *stu)
{
    printf("name: %s  number: %d  age: %d  source: %.1f\n",
        stu->name, stu->number, stu->age, stu->source);
}

// 打印链表
void display_list(list *list)
{
    listIter *iter = listGetIterator(list, AL_START_TAIL);
    listNode *node;
    while((node = listNext(iter)) != NULL) {
        print_student((student *)node->value);
    }
}

int main()
{
    list *lst = listCreate();       // 创建链表
    
    listAddNodeHead(lst, createStudent("Jim Weiss", 12, 23, 78.5));
    listAddNodeHead(lst, createStudent("Tom Jesson", 12, 23, 78.5));
    listAddNodeHead(lst, createStudent("Make Houn", 12, 23, 78.5));
    
    display_list(lst);
 
}
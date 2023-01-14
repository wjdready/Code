#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct hashNode {
    // 关键字
    char *key;
    // 关键字对应值
    void *value;
    // 指向下一个哈希节点
    struct hashNode *next;
}hashNode;

typedef struct hashTable {
    // 一个指针指向数组，数组存放指针，该指针指向哈希节点
    hashNode **table;
    unsigned long size;
}hashTable;


// 新建一个hash节点，新建失败返回NULL
hashNode *newHashNode(const char *key, void *value)
{
    hashNode *node = (hashNode*)malloc(sizeof(hashNode));
    if(node == NULL)
        return NULL;
    node->key = strdup(key);
    if(node->key == NULL) {
        free(node);
        return NULL;
    }
    node->value = value;
    node->next = NULL;

    return node;
}


hashTable *hashCreate(int size)
{
    hashTable *hash = (hashTable*)malloc(sizeof(hashTable));
    if(hash == NULL)
        return NULL;

    hash->table = (hashNode**)malloc(sizeof(hashNode*)*size);
    if(hash->table == NULL) {
        free(hash);
        return NULL;
    }
    hash->size = size;

    // 初始化hash数组
    for(int i=0; i<size; i++) {
        hash->table[i] = NULL;
    }

    return hash;
}

// 根据键值获取对于数组的index
unsigned long hashIndex(hashTable *hash, const char *key)
{
    unsigned long index = 0;
    while(*key)
        index = index*33 + *key++;
    return index%hash->size;
}

// 根据key查找哈希表，如果找到返回该节点，否则返回NULL
hashNode *hashSearch(hashTable *hash, const char *key)
{
    hashNode *node;
    unsigned long index;
    index = hashIndex(hash, key);
    for(node=hash->table[index]; node; node=node->next)
        if(!strcmp(node->key, key)) // 关键字相等
            return node;
    return NULL;
}



// 定义一个插入结点的方法，首先是查看该key值的结点是否存在
// k如果存在则更改value值就好，如果不存在，则插入新结点。
bool hashInsert(hashTable *hash, const char *key, void *value)
{
    hashNode *node;
    unsigned long index;
    // 如果没找到，则新建一个节点
    if((node = hashSearch(hash, key)) == NULL) {
        // 重新获取索引值
        index = hashIndex(hash, key);
        if((node = newHashNode(key, value)) == NULL)
            return false;   // 新建内存失败
        
        // 插入对应的哈希表的链表分支头结点
        node->next = hash->table[index];
        hash->table[index] = node;
        return true;
    }
    // 对应键值已经存有节点，则键值不变，释放value旧值，指向成新的
    free(node->value);
    node->value = value;
    return true;
}


void *hashNodeValue(hashNode *node)
{
    return node->value;
}

void hashRelease(hashTable *hash)
{
    hashNode *node;
    // 释放数组中的所有链表，以及链表节点指向的内存
    for(int i=0; i<hash->size; i++) {
        while(hash->table[i] != NULL) {
            node = hash->table[i]->next;
            // 释放哈希节点所链接的内存
            free(hash->table[i]->key);
            free(hash->table[i]->value);
            // 释放节点本身
            free(hash->table[i]);
            hash->table[i] = node;
        }
    }
    // 释放指针数组
    free(hash->table);
    // 释放哈希表本身
    free(hash);
}

unsigned long hashGetTableSize(hashTable *hash)
{
    return hash->size;
}

void printHash(hashTable *hash)
{
    
    hashNode *node;
    for(int i=0; i<hash->size; i++) {
        printf("[%2d]-->", i);
        // 指向第一个元素
        node = hash->table[i];  
        while( node != NULL) {
            printf("[%s]-->", hash->table[i]->key);
            node = node->next;
        }
        printf("null\n");
    }
}




int main()
{
    int example();
    example();

    return 0;
}


typedef struct student {
    char name[20];
    int age;
}student;


void printStudent(void *stu)
{
    student *st = (student*)stu;
    printf("name %s ", st->name);
    printf("age %d ", st->age);
    printf("\n");
}


student *newStudent(const char *name, int age)
{
    student *stu = (student *)malloc(sizeof(student));
    stu->age = age;
    strcpy(stu->name, name);
    return stu;
}


int example()
{
    hashTable *hash = hashCreate(3);

    // 插入hash表
    hashInsert(hash, "Jim", newStudent("李苗苗", 22));
    hashInsert(hash, "Peter", newStudent("赵庄闷", 23));
    hashInsert(hash, "Mike", newStudent("林加油", 27));
    hashInsert(hash, "Docter", newStudent("梦网纱", 25));
    hashInsert(hash, "Hacker", newStudent("丁克族", 34));
    printHash(hash);

    hashNode *node = hashSearch(hash, "Hacker");

    if(node)
        printStudent(hashNodeValue(node));

    hashRelease(hash);

    printf("ret\n");
    return 0;
}
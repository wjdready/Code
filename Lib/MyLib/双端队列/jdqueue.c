#include "jdqueue.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * 双端队列节点
 */
typedef struct queueNode {

    // 前置节点
    struct queueNode *prev;

    // 后置节点
    struct queueNode *next;

    // 节点的值
    void *value;

} queueNode;


/**
 * 双端队列迭代器
 */
typedef struct queueIter {
    // 当前迭代到的节点
    queueNode *next;

    // 迭代的方向
    int direction;

} queueIter;


/*
 * 双端队列结构
 */
typedef struct queue {

    // 队列头节点
    queueNode *front;

    // 队列尾节点
    queueNode *back;

    // 节点值复制函数
    void *(*dup)(void *ptr);

    // 节点值释放函数
    void (*free)(void *ptr);

    // 节点值对比函数
    int (*match)(void *ptr, void *key);

    // 队列所包含的节点数量
    unsigned long len;
} queue;


/**
 * 创建一个新的队列
 * 
 * 创建成功返回队列，失败返回 NULL
 * 
 */
queue *queueCreate(void)
{
    struct queue *queue;

    // 分配内存
    if((queue = malloc(sizeof(*queue))) == NULL)
        return NULL;
    
    // 初始化属性
    queue->front = queue->back = NULL;
    queue->len = 0;
    queue->dup = NULL;
    queue->free = NULL;
    queue->match = NULL;

    return queue;
}

/*
 * 将一个包含有给定值指针 value 的新节点添加到队列
 *
 * 如果为新节点分配内存出错，那么不执行任何动作，仅返回 NULL
 *
 * 如果执行成功，返回传入的队列指针
 *
 */
queue *queuePush(queue *queue, void *value)
{
    queueNode *node = malloc(sizeof(queueNode));
    if(node == NULL)
        return NULL;
    
    node->value = value;

    // 添加到空队列
    if(queue->len == 0) {
        queue->front = queue->back = node;
        node->prev = node->next = NULL;
    // 添加到非空队列,从队尾插入
    } else {
        node->next = queue->back;
        node->prev = NULL;
        queue->back->prev = node;
        queue->back = node;
    }

    // 更新队列节点数
    queue->len++;

    return queue;
}


/**
 * 从队列的队头弹出元素, 同时释放队头节点
 * 
 * 如果队列为空，则返回NULL
 * 
 */ 
void *queuePop(queue *queue)
{
    if(queue->len == 0)
        return NULL;
    
    // 从队头弹出
    queueNode *node = queue->front;
    queue->front = queue->front->prev;

    // 如果弹出后队列为空
    if(queue->front == NULL)
        queue->front = queue->back = NULL;
    else 
        queue->front->next = NULL;

    queue->len--;

    // 释放节点并返回 value
    void *value = node->value;
    free(node);

    return value;
}

/**
 * 获取队列头的元素值
 * 
 * 若队列为空，则返回NULL
 * 
 */ 
void *queueFront(queue *queue)
{
    if(queue->len == 0)
        return NULL;
        
    return queue->front->value;
}

/**
 * 获取队列尾的元素值
 * 
 * 若队列为空，则返回NULL
 * 
 */ 
void *queueBack(queue *queue)
{
    if(queue->len == 0)
        return NULL;

    return queue->back->value;
}


/**
 * 判断队列是否为空
 */ 
int queueEmpty(queue *queue)
{
    return queue->len == 0;
}

/**
 * 获取队列大小
 */ 
unsigned long queueSize(queue *queue)
{
    return queue->len;
}


/**
 * 释放整个队列，以及队列中的所有节点
 */ 
void queueRelease(queue *queue)
{
    unsigned long len;
    queueNode *current, *next;

    // 指向指向队尾
    current = queue->back;
    // 遍历整个队列
    len = queue->len;
    while(len--) {
        next = current->next;

        // 如果有设置释放函数，那么调用它
        if(queue->free)
            queue->free(current->value);

        // 释放节点结构
        free(current);
        current = next;
    }
    queue->len = 0;
    // 释放队列结构
    free(queue);
}


/*
 * 为给定队列创建一个迭代器，
 * 之后每次对这个迭代器调用 listNext 都返回被迭代到的队列节点的值
 *
 * direction 参数决定了迭代器的迭代方向：
 *  JQ_START_FRONT  ：从队列头向队列尾迭代
 *  JQ_START_BACK   ：从队列尾向队列头迭代
 *
 */
queueIter *queueGetIterator(queue *queue, int direction)
{
    // 为迭代器分配内存
    queueIter *iter = malloc(sizeof(*iter));
    if(iter == NULL)
        return NULL;

    // 根据迭代方向，设置迭代起点
    if(direction == JQ_START_FRONT)
        iter->next = queue->front;
    else
        iter->next = queue->back;
    
    // 记录迭代方向
    iter->direction = direction;

    return iter;   
}


/*
 * 返回迭代器当前所指向的节点。
 *
 * 函数要么返回一个节点，要么返回 NULL ，常见的用法是：
 *
 * iter = listGetIterator(list,<direction>);
 * while ((value = listNext(iter)) != NULL) {
 *     doSomethingWith(value);
 * }
 *
 * T = O(1)
 */
void *queueNext(queueIter *iter)
{
    queueNode *current = iter->next;

    if(current == NULL) 
        return NULL;

    // 根据方向选择下一个节点
    if(iter->direction == JQ_START_FRONT)
        iter->next = current->prev;
    else
        iter->next = current->next;
    
    return current->value;
}


/*
 * 将迭代器的方向重新设置为 JQ_START_FRONT ，
 * 并将迭代指针重新指向表头节点。
 *
 * T = O(1)
 */
void queueRewind(queue *queue, queueIter *iter)
{
    iter->next = queue->front;
    iter->direction = JQ_START_FRONT;
}

/*
 * 将迭代器的方向重新设置为 JQ_START_BACK ，
 * 并将迭代指针重新指向表头节点。
 *
 * T = O(1)
 */
void queueRewindBack(queue *queue, queueIter *iter)
{
    iter->next = queue->back;
    iter->direction = JQ_START_BACK;
}


/**
 * 释放队列迭代器
 */
void queueReleaseIterator(queueIter *iter)
{
    free(iter);
}


/**
 * 设置将链表的 value 值释放函数
 */ 
void queueSetFreeMethod(queue *queue, void (*free)(void *ptr))
{
    queue->free = free;
}

#ifndef __JDQUEUE_H__
#define __JDQUEUE_H__


typedef struct queue queue;
typedef struct queueIter queueIter;

/* Prototypes */
queue *queueCreate(void);
queue *queuePush(queue *queue, void *value);
void *queuePop(queue *queue);
void *queueFront(queue *queue);
void *queueBack(queue *queue);
int queueEmpty(queue *queue);
unsigned long queueSize();
void queueRelease(queue *queue);

queueIter *queueGetIterator(queue *queue, int direction);
void *queueNext(queueIter *iter);
void queueReleaseIterator(queueIter *iter);
void queueRewind(queue *queue, queueIter *iter);
void queueRewindBack(queue *queue, queueIter *iter);

void queueSetFreeMethod(queue *queue, void (*free)(void *ptr));


/*
 * 迭代器进行迭代的方向
 */
// 从表头向表尾进行迭代
#define JQ_START_FRONT 0
// 从表尾到表头进行迭代
#define JQ_START_BACK 1


#endif
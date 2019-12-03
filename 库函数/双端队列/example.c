#include <stdio.h>
#include "jdqueue.h"
#include <stdlib.h>

int main()
{
    queue *q = queueCreate();

    char s1[] = "Hello";
    char s2[] = "World";
    char s3[] = "Jim";

    queuePush(q, s1);
    queuePush(q, s2);
    queuePush(q, s3);
    
    printf("输入了三个数据，正向遍历\n");
    queueIter *iter = queueGetIterator(q, JQ_START_FRONT);
    char *data;
    while((data = queueNext(iter)) != NULL) {
        printf("%s \n", data);
    }

    printf("接着反向遍历\n");
    queueRewindBack(q, iter);
    while((data = queueNext(iter)) != NULL) {
        printf("%s \n", data);
    }

    
    printf("当前队列长度 len %ld\n", queueSize(q));
    printf("弹出队头 pop %s\n", (char *)queuePop(q));
    printf("弹出后队列长度 len %ld\n", queueSize(q));

    // 释放队列和迭代器
    queueRelease(q);
    queueReleaseIterator(iter);
    printf("len %ld\n", queueSize(q));

    return 0;

}
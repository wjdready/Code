#ifndef __THREAD_H__
#define __THREAD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct
{
    uint32_t *sp;
    void (*entry)(void *);
    int stack_size;
    const char *name;
    uint32_t tick;
} thread_t;

thread_t *thread_create(const char *name, void (*entry)(void *), void *param, void *stack, int stack_size);

#ifdef __cplusplus
}
#endif

#endif


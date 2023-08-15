#include <stdio.h>
#include <stdlib.h>

typedef struct memdebug
{
    void *p;
    size_t size;
    int line;
    const char *file;
    struct memdebug *next;
    struct memdebug *prev;
} memdebug_t;

static memdebug_t *head;

void *memdebug_alloc(size_t size, const char *file, int line)
{
    memdebug_t *mem = (memdebug_t *)malloc(size + sizeof(memdebug_t));
    if (mem == NULL)
        return NULL;

    mem->p = mem + 1;
    mem->size = size;
    mem->file = file;
    mem->line = line;
    mem->next = mem->prev = NULL;

    // printf("malloc: size %d %s:%d\n", size, file, line);

    if (head == NULL)
        head = mem;
    else
    {
        if (head->prev)
        {
            head->prev->next = mem;
            mem->prev = head->prev;
        }
        head->prev = mem;
        mem->next = head;
        head = mem;
    }

    return mem->p;
}

void memdebug_free(void *p, const char *file, int line)
{
    if (p == NULL)
        return;

    memdebug_t *mem = (memdebug_t *)(p - sizeof(memdebug_t));
    if (mem->p != p)
        return;

    // printf("free: size %d %s:%d, malloc in %s:%d\n", mem->size, file, line, mem->file, mem->line);

    if (head == mem && head->next == NULL)
        head = NULL;
    else if (head == mem)
    {
        head->next->prev = NULL;
        head = head->next;
    }
    else
    {
        if (mem->prev)
            mem->prev->next = mem->next;
        if (mem->next)
            mem->next->prev = mem->prev;
    }

    mem->p = NULL;
    free(mem);
}

void memdebug_print(void)
{
    for (memdebug_t *mem = head; mem; mem = mem->next)
        printf("not free: size %d in %s:%d\n", mem->size, mem->file, mem->line);
}

#ifndef __MEMDEBUG_H__
#define __MEMDEBUG_H__
#include <stdint.h>

void *memdebug_alloc(size_t size, const char *file, int line);
void memdebug_free(void *p, const char *file, int line);
void memdebug_print(void);

#define malloc(size) memdebug_alloc(size, __FILE__, __LINE__)
#define free(p) memdebug_free(p, __FILE__, __LINE__)

#endif

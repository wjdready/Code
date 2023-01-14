#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

char *vasprintf(const char *fmt, va_list ap)
{
    unsigned int first, second;
    char *p;
    va_list aq;

    va_copy(aq, ap);
    first = vsnprintf(NULL, 0, fmt, aq);
    va_end(aq);

    p = malloc(first + 1);
    if (!p)
        return NULL;

    second = vsnprintf(p, first + 1, fmt, ap);
    if(first != second)
        printf("different return values (%u and %u) from vsnprintf(\"%s\", ...)",
            first, second, fmt);

    return p;
}


void debug_helper(const char *TAG, const char *fmt, va_list args)
{
    char *str = vasprintf(fmt, args);
    if(str)
    {
        printf("%s: %s\n", TAG, str);
        free(str);
    }
}

void debug(const char *TAG, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    debug_helper(TAG, fmt, args);

    va_end(args);
}

#define DEBUG_ERROR(...) debug("ERROR", __VA_ARGS__)

int main(int argc, char const *argv[])
{
    DEBUG_ERROR("Something wrong!");
    
    const char *TAG = __FUNCTION__;
    debug(TAG, "Some information");

    return 0;
}

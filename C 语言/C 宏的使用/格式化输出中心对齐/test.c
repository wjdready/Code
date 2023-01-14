#include <stdio.h>
#include <string.h>

#define LEFT(str, w) \
    ({int m = w + strlen(str); m % 2 ? (m + 1) / 2 : m / 2;})

#define RIGHT(str, w) \
({ int m = w - strlen(str); m % 2 ? (m - 1) / 2 : m / 2; })

#define STR_CENTER(str, width) \
    LEFT(str, width), str, RIGHT(str, width), ""

#define PRINTF_CENTER(width, start, fmt, end, ...) ({ \
    int n = snprintf(NULL, 0, fmt, __VA_ARGS__);     \
    int m = width - n;                               \
    int left = m % 2 ? (m + 1) / 2 : m / 2;          \
    int right = m % 2 ? (m - 1) / 2 : m / 2;         \
    printf(start "%*s" fmt "%*s" end, left, "",      \
           __VA_ARGS__, right, "");                  \
})

#define MYFORMAT_CENTER(width, fmt, ...)  \
    PRINTF_CENTER(40, "[", fmt  , "]\n", __VA_ARGS__)

int main(int argc, char const *argv[])
{
    printf("%*s%*s\n\n", STR_CENTER("--- Hello World ---", 40));
    printf("[%*s%*s]\n", STR_CENTER("I am okay today", 40));
    
    MYFORMAT_CENTER(40, "%d, e is %f", 1, 2.71828);
    MYFORMAT_CENTER(40, "%d, pi is %f", 2, 3.1415926);
    MYFORMAT_CENTER(40, "%s %d.", "This is such a long string that it exceeds the given size:", 40);

    return 0;
}

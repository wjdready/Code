#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void initialise_monitor_handles(void);

int main(int argc, char const *argv[])
{
    // initialise_monitor_handles();
    char *data = malloc(1);
    if (data == NULL)
    {
        return -1;
    }

    memset(data, 0, 1);
    printf("Hello World %s\n", data);

    return 0;
}

// // extern void __set_heaptop(void *ptr);
// char *_sbrk(int adj)
// {
//     extern char end;
//     static char *heap = &end;
//     char *rv = heap;

//     heap += adj;
//     // __set_heaptop(heap);
//     return rv;
// }
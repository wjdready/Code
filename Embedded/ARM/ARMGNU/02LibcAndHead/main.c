#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void initialise_monitor_handles(void);

int aaa = 12345;

int main(int argc, char const *argv[])
{
    initialise_monitor_handles();

    char *data = malloc(64);
    if (data == NULL)
        return -1;

    strcpy(data, "I am Jack");
    printf("Hello World, %s, aaa is %d\n", data, aaa);

    return 0;
}

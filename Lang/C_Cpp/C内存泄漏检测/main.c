#include <stdio.h>
#include <stdlib.h>
#include "memdebug.h"

void test(void)
{
    char *s;
    for (int i = 0; i < 10; i++)
    {
        s = (char*)malloc(i*10);
        if(i % 2 == 0)
            free(s);
    }

    char buf[1011];
    free(buf);
    free(s);
    free(s);
}

int main(int argc, char const *argv[])
{
    test();
    memdebug_print();

    return 0;
}

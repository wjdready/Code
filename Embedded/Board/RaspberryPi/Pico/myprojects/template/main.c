#include <stdio.h>
#include "pico/stdlib.h"

void test_command_handler(void)
{
    static char input_string[1024];
    int test_command(char *input_string);

    gets(input_string);

    test_command(input_string);

    printf("\n#sh ");
}

int main()
{
    stdio_init_all();

    printf("\n#sh ");

    while (1)
    {
        test_command_handler();
    }
}

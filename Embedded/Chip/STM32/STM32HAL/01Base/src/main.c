#include <stdio.h>

extern void initialise_monitor_handles(void);
int test_command(char *input_string);

int main(int argc, char const *argv[])
{
    initialise_monitor_handles();

    // for (int i = 0; i < 100000; i++)
    // {
    //     printf("Hello World %d\n", i);
    // }

    char buf[256];

   while(1)
    {
        gets(buf);
        // printf("you input is %s\n", buf);
        test_command(buf);
    }
    return 0;
}


/* __TEST_COMMAND__START__ */
#include "test_command.h"
static int asdasdasd(int argc, char *argv[])
{
    printf("Hello World GOgo\n");
    
    FILE *fp  = fopen("goo.txt", "w");
    fwrite("H", 1, sizeof("H"), fp);
    fclose(fp);

    return 0;
}
EXPORT_TEST_COMMAND(asdasdasd, "name", "中文的强大指出");
/* __TEST_COMMAND__END__ */

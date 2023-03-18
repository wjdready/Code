#include <stdio.h>

extern void initialise_monitor_handles(void);
int test_command(char *input_string);

int main(int argc, char const *argv[])
{
    initialise_monitor_handles();
    char buf[256];

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();

    while (1)
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

    FILE *fp = fopen("goo.txt", "w");
    fwrite("Hello World", 1, sizeof("Hello World"), fp);
    fclose(fp);

    return 0;
}
EXPORT_TEST_COMMAND(asdasdasd, "cmd_test", "test Command");
/* __TEST_COMMAND__END__ */

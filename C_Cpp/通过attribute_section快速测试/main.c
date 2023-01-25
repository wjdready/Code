#include <stdio.h>
#include "test_command.h"
#include "pthread.h"
#include <unistd.h>

void *test_command_task(void * arg)
{
    test_command_start();
    while (1)
    {
        sleep(1);
    }
}


int main(int argc, char const *argv[])
{
    printf("Hello World\n");
    
    pthread_t t;
    pthread_create(&t, NULL, test_command_task, NULL);
    while (1)
    {
        sleep(1);
    }
    

    return 0;
}


/* __TEST_COMMAND__START__ */
#include "test_command.h"
static int ret1(int argc, char *argv[])
{
    printf("Hello World\n");
    
    return 1;
}
EXPORT_TEST_COMMAND(ret1, "ret1", "return 2");
/* __TEST_COMMAND__END__ */


/* __TEST_COMMAND__START__ */
#include "test_command.h"
static int ret2(int argc, char *argv[])
{
    printf("Hello World\n");
    
    return 2;
}
EXPORT_TEST_COMMAND(ret2, "ret2", "return 2");
/* __TEST_COMMAND__END__ */

#include <stdio.h>
#include "test_command.h"


int main(int argc, char const *argv[])
{
    char input_string[1024];

    while (1)
    {
        printf("\n#sh ");
        fgets(input_string, sizeof(input_string), stdin);
    
        test_command(input_string);
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

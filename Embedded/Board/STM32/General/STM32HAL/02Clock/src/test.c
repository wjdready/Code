#include <stdio.h>

void myasfunc(void);

/* __TEST_COMMAND__START__ */
#include "test_command.h"
static int tetsttt(int argc, char *argv[])
{
    printf("Now we call myasfunc\n");
    
    myasfunc();

    return 0;
}
EXPORT_TEST_COMMAND(tetsttt, "cc", "my as func call");
/* __TEST_COMMAND__END__ */

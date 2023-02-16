#include "test_command.h"
#include <stdio.h>
#include "uartsh.h"
#include "log.h"
#include <string.h>

#ifdef __ARMCC_VERSION  /* ARM C Compiler */
    extern test_command_t test_command_section$$Base;
    extern test_command_t test_command_section$$Limit;
    test_command_t *test_command_section_begin = &(test_command_section$$Base);
    test_command_t *test_command_section_end = &(test_command_section$$Limit);
#elif defined (__GNUC__)
    extern test_command_t __start_test_command_section;
    extern test_command_t __stop_test_command_section;
    test_command_t *test_command_section_begin = &__start_test_command_section;
    test_command_t *test_command_section_end = &__stop_test_command_section;
#else 
    #error "The platform is not supported"
#endif

#define foreach_command(item) \
    for (test_command_t *item = test_command_section_begin; \
    item != test_command_section_end; item++)

int uartshCommand_test(int argc, char *argv[])
{
    if (argc == 1 || strncmp(argv[1], "help", 4) == 0)
    {
        foreach_command(entry)
        {
            printf("    %-15s\t -- %s\n", entry->name, entry->desc);
        }

        return 0;
    }

    foreach_command(entry)
    {
        if (strcmp(entry->name, argv[1]) == 0)
        {
            entry->fun(argc - 1, argv + 1);
            return 0;
        }
    }

    printf("命令 %s 未找到\n", argv[1]);

    return 0;
}

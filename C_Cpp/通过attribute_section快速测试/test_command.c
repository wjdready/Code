#include "test_command.h"
#include <stdio.h>
#include <string.h>

/* 获取函数列表所在的内存区间 */
extern test_command_t __start_test_command_section;
extern test_command_t __stop_test_command_section;
static test_command_t *test_command_section_begin = &__start_test_command_section;
static test_command_t *test_command_section_end = &__stop_test_command_section;

#define foreach_command(item) \
    for (test_command_t *item = test_command_section_begin; \
    item != test_command_section_end; item++)

/* 添加内部命令 */
TEST_FUN_ADD_INTERNAL(last_result, "r", "Get last command exec result");
TEST_FUN_ADD_INTERNAL(all_command, "a", "Show all supported command");

static int get_args(char command_string[], char const tokens[], int argc_max, char* argv[])
{
	int argc = 0;
	char* token = strtok(command_string, tokens);

	while(token != NULL)
	{
		argv[argc++] = token;
		if( argc >= argc_max )
			break;

		token = strtok(NULL, tokens);
	}

	return argc;
}

int test_command_start(void)
{
    char command[1024];
    char *argv[32] = {0};
    int result = 0;
    
    while (1)
    {
        printf("\nsh# ");
        gets(command);

        int argc = get_args(command, " \n", 31, argv);

        if(argc == 0) continue;

        if (strcmp(argv[0], "a") == 0 || strcmp(argv[0], "help") == 0)
        {
            foreach_command(entry)
            {
                printf("    %-15s\t -- %s\n", entry->name, entry->desc);
            }

            continue;
        }

        /* 打印上一次命令执行结果 */
        if (strcmp(argv[0], "r") == 0)
        {
            printf("%d\n", result);
            continue;
        }

        foreach_command(entry)
        {
            if (strcmp(entry->name, command) == 0)
            {
                result = entry->fun(argc, argv);
                break;
            }
        }
    }

    return 0;
}


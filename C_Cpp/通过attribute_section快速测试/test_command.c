#include "test_command.h"
#include <stdio.h>
#include <string.h>

#ifdef __ARMCC_VERSION /* ARM C Compiler */
extern test_command_t test_command_section$$Base;
extern test_command_t test_command_section$$Limit;
test_command_t *test_command_section_begin = &(test_command_section$$Base);
test_command_t *test_command_section_end = &(test_command_section$$Limit);
#elif defined(__GNUC__)
extern test_command_t __start_test_command_section;
extern test_command_t __stop_test_command_section;
test_command_t *test_command_section_begin = &__start_test_command_section;
test_command_t *test_command_section_end = &__stop_test_command_section;
#else
#error "The platform is not supported"
#endif

#define foreach_command(item)                                                                                          \
    for (test_command_t *item = test_command_section_begin; item != test_command_section_end; item++)

/* 快速定义内部命令, 内部命令什么也不做只是用来显示和提供帮助 */
#define TEST_FUN_ADD_INTERNAL(label, n, d)                                                                             \
    static int _##label(int argc, char **argv)                                                                         \
    {                                                                                                                  \
        return 0;                                                                                                      \
    }                                                                                                                  \
    EXPORT_TEST_COMMAND(_##label, n, d)

/* 添加内部命令 */
TEST_FUN_ADD_INTERNAL(last_result, "r", "Get last command exec result");
TEST_FUN_ADD_INTERNAL(all_command, "a", "Show all supported command");

/* 将给定字符串拆分成参数列表 */
static int test_command_get_args(char *command_string, const char *tokens, int argc_max, char **argv)
{
    int argc = 0;
    char *token = strtok(command_string, tokens);

    while (token != NULL)
    {
        argv[argc++] = token;
        if (argc >= argc_max)
            break;

        token = strtok(NULL, tokens);
    }

    return argc;
}

/**
 * @brief 处理给定的输入字符串
 *
 * 例如 echo "Hello"
 *
 * @param input_string 输入字符串
 *
 * @return int
 */
int test_command(char *input_string)
{
    char *argv[32] = {0};
    static int result = 0;

    int argc = test_command_get_args(input_string, " \n", 31, argv);
    if (argc == 0)
        return -1;

    if (strcmp(argv[0], "a") == 0 || strcmp(argv[0], "help") == 0)
    {
        foreach_command(entry) test_command_printf("    %-15s\t -- %s\n", entry->name, entry->desc);

        return 0;
    }

    /* 打印上一次命令执行结果 */
    if (strcmp(argv[0], "r") == 0)
    {
        test_command_printf("%d\n", result);
        return 0;
    }

    foreach_command(entry)
    {
        if (strcmp(entry->name, argv[0]) == 0)
        {
            result = entry->fun(argc, argv);
            return result;
        }
    }

    test_command_printf("Command %s not found\n", argv[0]);

    return -1;
}

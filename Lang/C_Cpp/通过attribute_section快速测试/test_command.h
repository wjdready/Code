#pragma once

typedef int (*command_func_t)(int, char **);

#define test_command_printf printf

typedef struct
{
    int (*fun)(int, char **);
    const char *name;
    const char *desc;
    char _tmp[8]; /* 32 位对齐 */
} test_command_t;

#define TEST_COMMAND_FUNC_DEFINE(f) int f(int argc, char **argv)

#define SECTION __attribute((used, section("test_command_section")))

/**
 * @brief 导出一个标准的测量命令
 *      一个标准的命令应该是形如 int command(int argc, char *argv[]);
 *      它能接收命令行参数, 并作出相应的处理, 同时还有返回值 int
 * 例如:
 *
 * static int my_test_command(int argc, char *argv[])
 * {
 *     printf("command %s, args number: %d\n", argv[0], argc);
 *
 *     return 0;
 * }
 * EXPORT_TEST_COMMAND(my_test_command, "mytest", "打印命令参数");
 *
 * 一般来说, 我们可以通过定义一个标准测试命令, 并处理输入的参数来对某一个上下文定义的函数进行测试
 */
#define EXPORT_TEST_COMMAND(f, n, d)                                                                                   \
    static test_command_t _##f SECTION = {                                                                             \
        .fun = f,                                                                                                      \
        .name = n,                                                                                                     \
        .desc = d,                                                                                                     \
    }

/**
 * @brief 将一个已有的函数添加到测量命令里
 *  即对一个已有的函数进行快速测试, 需要显式提供被测函数的参数
 *
 * 例如: 现有一个函数 add(int a, int b);
 *
 *      那么其导出到测试命令的写法为:
 *
 *      TEST_FUN_ADD(add, "add_test", "对函数 add() 进行测试, 参数为 (1,2)", 1, 2);
 *
 *      其中, 1, 2 是我们希望对函数进行测试的参数
 */
#define TEST_FUN_ADD(f, n, d, ...)                                                                                     \
    static int _##f(int argc, char **argv)                                                                             \
    {                                                                                                                  \
        f(__VA_ARGS__);                                                                                                \
        return 0;                                                                                                      \
    }                                                                                                                  \
    EXPORT_TEST_COMMAND(_##f, n, d)

/* -- function prototypes -- */

/**
 * @brief 处理给定的输入字符串
 *
 * 例如 echo "Hello"
 *
 * @param input_string 输入字符串
 *
 * @return int
 */
int test_command(char *input_string);

/* -- END OF function prototypes -- */

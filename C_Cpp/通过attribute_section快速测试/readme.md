### 通过 __attribute( (section(x)) ) 来对函数进行快速测试

先从简单代码开始，以下代码通过宏 `EXPORT` 来帮助我们快速定义一个 myfun_t 变量, 所以在` EXPORT(hello); `就定义了一个 `_hello` 变量, 于是我们可以在 main 中进行访问

```c
#include <stdio.h>

typedef struct
{
    void (*fun)(void);
    const char *desc;
} myfun_t;

#define EXPORT(x)               \
    static myfun_t _##x = {     \
        .fun = x                \
    }

void hello(void)
{
    printf("Hello\n");
}
EXPORT(hello);

int main(int argc, char const *argv[])
{
    _hello.fun();

    return 0;
}
```



__attribute 宏用来设置编译属性, `__attribute( (section(x)) )` 可以指定编译器将变量存放到指定的内存区域, 之后我们就可以使用 

```
extern type_t __start_<section_name>
extern type_t __stop_<section_name>
```

来进行访问,  `__start_<section_name>` 代表段的起始, 是一个内置标签, 就像 int a = 12; 中的 a 一样, 它的数据类型取决于我们如何看待它, 和很多变量标签一样, 通过 `&` 可以访问变量所在的地址, 所以访问段的的起始地址为 `&__start_mysection`

```c
#include <stdio.h>

#define MYSECTION __attribute((used, section("mysection")))

char mystr[] MYSECTION = "Good";
// int myvalue MYSECTION = 125;

int main(int argc, char const *argv[])
{
    extern char __start_mysection;
    printf("%s\n", &__start_mysection);
    
    // extern int __start_mysection;
    // printf("%d\n", myvalue);     // 125

    return 0;
}
```

### 测试代码

有了上面的基础, 就可以尝试写快速测试的代码了，一般来说我们希望快速测试一个函数, 编写完后在函数下面通过一个宏定义 TEST_FUNC_ADD 就可以将函数插入到测试代码的行列。

test_command.c

```c
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

#define foreach_command(item)                                                                              \
    for (test_command_t *item = test_command_section_begin; item != test_command_section_end; item++)

/* 快速定义内部命令, 内部命令什么也不做只是用来显示和提供帮助 */
#define TEST_FUN_ADD_INTERNAL(label, n, d)                                                                 \
    static int _##label(int argc, char **argv)                                                             \
    {                                                                                                      \
        return 0;                                                                                          \
    }                                                                                                      \
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
```

test_command.h

```c
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
#define EXPORT_TEST_COMMAND(f, n, d)                                                             \
    static test_command_t _##f SECTION = {                                                       \
        .fun = f,                                                                                \
        .name = n,                                                                               \
        .desc = d,                                                                               \
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
#define TEST_FUN_ADD(f, n, d, ...)                                                               \
    static int _##f(int argc, char **argv)                                                       \
    {                                                                                            \
        f(__VA_ARGS__);                                                                          \
        return 0;                                                                                \
    }                                                                                            \
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
```

main.c 

```c
#include <stdio.h>
#include "test_command.h"

int main(int argc, char const *argv[])
{
    char input_string[1024];

    while (1)
    {
        printf("\n#sh ");
        gets(input_string);
    
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

```

makefile

```makefile
# [MyProject]

default:
	gcc main.c test_command.c

run:
	./a.exe

clean:
	@rm ./a.exe 2> /dev/null || true 

globalclean:
	make clean
```


[参考文献](https://stackoverflow.com/questions/3633896/append-items-to-an-array-with-a-macro-in-c)


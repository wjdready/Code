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

test.h

```c
#pragma once

typedef struct
{
    void (*fun)(void);
    const char *name;
    const char *desc;
    char _tmp[8]; /* 32 位对齐 */
} test_command_t;

#define SECTION __attribute((used, section("myfun_section")))

#define EXPORT_TEST_COMMAND(f, n, d)       \
    static test_command_t _##f SECTION = { \
        .fun = f,                          \
        .name = n,                         \
        .desc = d,                         \
    }

#define TEST_FUNC_ADD(f, n, d, ...)           \
    static void _##f(void)                    \
    {                                         \
        int r = f(__VA_ARGS__);               \
        if (r == 0)                           \
            printf("Func %s TEST OK\n", n);   \
        else                                  \
            printf("%s TEST ERR %d\n", n, r); \
        return;                               \
    }                                         \
    EXPORT_TEST_COMMAND(_##f, n, d)

```

test.c

```c
#include "test.h"
#include <stdio.h>
#include <string.h>

/* 获取函数列表所在的内存区间 */
extern test_command_t __start_myfun_section;
extern test_command_t __stop_myfun_section;
test_command_t *myfun_section_begin = &__start_myfun_section;
test_command_t *myfun_section_end = &__stop_myfun_section;

#define foreach_command(item) \
    for (test_command_t *item = myfun_section_begin; \
    item != myfun_section_end; item++)

void func1(void)
{
    printf("func1: hello\n");
}
EXPORT_TEST_COMMAND(func1, "print", "To print hello");

int func_test(int max)
{
    if (max < 10)
        return 0;

    return -1;
}
TEST_FUNC_ADD(func_test, "less", "Check less than 10", 8);

int main(int argc, char const *argv[])
{
    char command[256];

    while (1)
    {
        printf("\nsh# ");
        gets(command);

        if (strncmp(command, "help", 4) == 0)
        {
            foreach_command(entry)
            {
                printf("    %-15s\t -- %s\n", entry->name, entry->desc);
            }

            continue;
        }

        foreach_command(entry)
        {
            if (strcmp(entry->name, command) == 0)
            {
                entry->fun();
                break;
            }
        }
    }

    return 0;
}
```

然而上述功能只能对 GCC 平台有效, 如果是 ARMCC 或是其他平台, 因为编译器不同, 方法可能不一样, 为了跨平台, 就不得不添加平台检测的宏, 比如将下面的代码替换获取 myfun_section 所在的内存区间部分即可支持 ARMCC 平台。

```c
#ifdef __ARMCC_VERSION  /* ARM C Compiler */
    extern test_command_t myfun_section$$Base;
    extern test_command_t myfun_section$$Limit;
    test_command_t *myfun_section_begin = &(myfun_section$$Base);
    test_command_t *myfun_section_end = &(myfun_section$$Limit);
#elif defined (__GNUC__)
    extern test_command_t __start_myfun_section;
    extern test_command_t __stop_myfun_section;
    test_command_t *myfun_section_begin = &__start_myfun_section;
    test_command_t *myfun_section_end = &__stop_myfun_section;
#else
    #error "The platform is not supported"
#endif
```

[参考](https://stackoverflow.com/questions/3633896/append-items-to-an-array-with-a-macro-in-c)
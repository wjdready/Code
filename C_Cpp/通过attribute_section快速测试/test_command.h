#pragma once

typedef int (*command_func_t)(int, char **);

typedef struct
{
    int (*fun)(int, char **);
    const char *name;
    const char *desc;
    char _tmp[8]; /* 32 位对齐 */
} test_command_t;

#define TEST_COMMAND_FUNC_DEFINE(f) int f(int argc, char **argv)

#define SECTION __attribute((used, section("test_command_section")))
#define EXPORT_TEST_COMMAND(f, n, d)       \
    static test_command_t _##f SECTION = { \
        .fun = f,                          \
        .name = n,                         \
        .desc = d,                         \
    }

#define TEST_FUN_ADD_INTERNAL(label, n, d)       \
    static int _##label(int argc, char **argv) \
    {                                          \
        return 0;                              \
    }                                          \
    EXPORT_TEST_COMMAND(_##label, n, d)

#define TEST_FUN_ADD(f, n, d, ...)         \
    static int _##f(int argc, char **argv) \
    {                                      \
        f(__VA_ARGS__);                    \
        return 0;                          \
    }                                      \
    EXPORT_TEST_COMMAND(_##f, n, d)

#define TEST_FUN_ADD_AWAJSON(f, n, d, pdev, ...) \
    static int _##f(int argc, char **argv)       \
    {                                            \
        f(pdev, argc, argv);                     \
        return 0;                                \
    }                                            \
    EXPORT_TEST_COMMAND(_##f, n, d)

int test_command_start(void);

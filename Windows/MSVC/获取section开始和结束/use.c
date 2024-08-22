#include <stdio.h>

typedef void(*pfunc_t)(void);

typedef struct
{
    int startflag;
    int (*fun)(int, char**);
    const char* name;
    const char* desc;
    int endflag;
} test_command_t;

#pragma section(".mine$a", read)
__declspec(allocate(".mine$a")) const void* InitSegStart;

#pragma section(".mine$m", read)

#pragma section(".mine$z", read)
__declspec(allocate(".mine$z")) const void* InitSegEnd;

void InitializeObjects()
{
    const test_command_t** x = (test_command_t**)&InitSegStart;
    const test_command_t** y = (test_command_t**)&InitSegEnd;

    printf("%p\n", x);
    printf("%p\n", y);
    for (++x; x < y; ++x)
    {
        test_command_t* p = *x;
        printf("--> %p\n", p);

        if (p == NULL || p->startflag != 0xdeadbeef || p->endflag != 0xdeadc0de)
            continue;

        printf("---> %s %s\n", (*x)->name, (*x)->desc);
        // (*x)->fun(1, "fff");
    }
}

#define SECTION __declspec(allocate(".mine$m"))
#define EXPORT_TEST_COMMAND(f, n, d)                \
    static test_command_t _##f = {          \
        .startflag = 0xdeadbeef,                    \
        .fun = f,                                   \
        .name = n,                                  \
        .desc = d,                                  \
        .endflag = 0xdeadc0de                       \
    };                                              \
    static SECTION test_command_t *_##f##_s = &_##f; \


static int my_test_command(int argc, char *argv[])
{
    printf("my_test_command");

    return 0;
}

// 定义命令
static test_command_t _my_test_command = {
    .startflag = 0xdeadbeef,
    .fun = my_test_command,
    .name = "mytest",
    .desc = "打印命令参数",
    .endflag = 0xdeadc0de
};

// 将指针放入指定段
static __declspec(allocate(".mine$m")) test_command_t* _my_test_command_s = &_my_test_command;
static test_command_t* hehe = &_my_test_command_s;

static __declspec(allocate(".mine$m")) test_command_t* _my_test_command_s2 = &_my_test_command;
static test_command_t* hehe2 = &_my_test_command_s2;

int main()
{
    InitializeObjects();
    printf("done\n");
}

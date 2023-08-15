#include "thread.h"
#include "cm3.h"
#include <stdint.h>
#include <stdio.h>
#include "demo.h"

thread_t *g_current_task;
thread_t *g_next_task;
thread_t *g_idle_task;

#define THREAD_MAX 25
static int thread_tb_index = 0;
static thread_t thread_tb[THREAD_MAX];

void thread_init(thread_t *th, const char *name, void (*entry)(void *), void *param, void *stack, int stack_size)
{
    th->name = name;
    th->sp = (uint32_t *)(stack + stack_size);
    th->stack_size = stack_size;
    th->entry = entry;
    th->tick = 0;

    *(--th->sp) = (uint32_t)(1 << 24); // XPSR, Thumb Mode
    *(--th->sp) = (uint32_t)entry;     // PC
    *(--th->sp) = (uint32_t)0x14;      // LR
    *(--th->sp) = (uint32_t)0x12;      // R12
    *(--th->sp) = (uint32_t)0x3;       // R3
    *(--th->sp) = (uint32_t)0x2;       // R2
    *(--th->sp) = (uint32_t)0x1;       // R1
    *(--th->sp) = (uint32_t)param;     // R0
    *(--th->sp) = (uint32_t)0x11;      // R11
    *(--th->sp) = (uint32_t)0x10;      // R10
    *(--th->sp) = (uint32_t)0x9;       // R9
    *(--th->sp) = (uint32_t)0x8;       // R8
    *(--th->sp) = (uint32_t)0x7;       // R7
    *(--th->sp) = (uint32_t)0x6;       // R6
    *(--th->sp) = (uint32_t)0x5;       // R5
    *(--th->sp) = (uint32_t)0x4;       // R4
}

thread_t *thread_create(const char *name, void (*entry)(void *), void *param, void *stack, int stack_size)
{
    if (thread_tb_index < THREAD_MAX)
    {
        thread_t *th = &(thread_tb[thread_tb_index++]);
        thread_init(th, name, entry, param, stack, stack_size);
    }
}

/* 触发线程切换 */
void thread_switch(void)
{
    MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}

/* 线程调度 */
void thread_sechd(void)
{
    g_next_task = g_idle_task;

    for (int i = 0; i < thread_tb_index; i++)
    {
        thread_t *t = thread_tb + i;
        if (t->tick == 0)
        {
            g_next_task = t;
            break;
        }
    }

    if (g_current_task != g_next_task)
        thread_switch();
}

void thread_tick_handler(void)
{
    for (int i = 0; i < thread_tb_index; i++)
    {
        thread_t *t = thread_tb + i;
        if (t->tick > 0)
            t->tick--;
    }

    thread_sechd();
}

void thread_delay(int tick)
{
    g_current_task->tick = tick;
    thread_sechd();
}

void thread_entry(void *arg)
{
    printf("%s start\n", (char *)arg);

    while (1)
    {
        printf("Hello in %s\n", (char *)arg);
        thread_delay(5);
    }
}

void thread_entry2(void *arg)
{
    printf("%s start\n", (char *)arg);

    while (1)
    {
        printf("Hello in %s\n", (char *)arg);
        thread_delay(3);
        demo();
    }
}

void idle_thread_entry(void *arg)
{
    /**
     * @note !!! 开始写的时候因为这一行，导致代码一直进入 hard fault。当时不知道是什么原因。
     * 现象是代码总是运行完一轮调度后到第二轮时死机。
     *
     * 排除了半天，通过和参考代码的比对以及一行一行的替换，最后才发现，竟然是因为这里多插入了
     * printf("%s start\n", (char*)arg);
     * 把该行去掉就好了，另外当把 start 去掉也好了。
     *
     * 这时才意识到原来是 ILDE 线程给的栈太小了，只有 512字节，而标准库 printf 函数的执行似乎需要更多的占空间，大概是
     * 512 多一点 就差几个字节。这也是为什么把打印参数改小就能跑。
     *
     * 另外当时其实已经差不多发现问题了，但是没往栈溢出这方面想。因为当其他线程 delay 结束，打断
     * idle_thread_entry 进入新一轮调度时，发现切换的 g_current_task 的 sp 指向了很奇怪的地方。大概就是因为 idle
     * 栈溢出，错误修改了 其他线程的栈空间数据，导致线程切换时载入栈中的寄存器值是错误的，从而引发 hard fault.
     *
     */
    printf("%s start\n", (char *)arg);

    while (1)
    {
        // printf("RUN ilde thread\n");
    }
}

#define create_static_thread(name, entry, stack_size)                                                                  \
    {                                                                                                                  \
        static char stack[stack_size];                                                                                 \
        thread_create(name, entry, (void*)name, stack, sizeof(stack));                                                 \
    }

void thread_module_init(void)
{
    create_static_thread("thread1", thread_entry, 4096);
    create_static_thread("thread2", thread_entry2, 4096);
    // create_static_thread("thread3", thread_entry, 1024);
    // create_static_thread("thread4", thread_entry, 1024);
    // create_static_thread("thread5", thread_entry, 1024);
    // create_static_thread("thread6", thread_entry, 1024);
    // create_static_thread("thread7", thread_entry, 1024);

    g_next_task = thread_tb + 0;

    static char idle_stack[1024];
    static thread_t idle_thread;
    thread_init(&idle_thread, "idle_thread", idle_thread_entry, (void *)"idle_thread", idle_stack, sizeof(idle_stack));
    g_idle_task = &idle_thread;
}

---
mytags: myblog
title: STM32 串口的使用
date: 2023-02-16 18:59:07
categories: [STM32, 串口]
tags: [STM32, 串口]
---

本文主要介绍 STM32 中串口的使用
<!-- more -->

串口是嵌入式中调试的好工具, 一般来说最先解决的应当是串口, 只有串口调好了, 后面的开发和单元测试写起来才舒服.

处理串口的一个比较经典的做法是采用 `DMA + 中断 + 环形缓冲区` 的形式, 环形缓冲区有一个读指针和一个写指针.

两个指针保证了缓冲区的数据读写操作是安全的, 即使在读的过程中触发了数据接收中断使程序进入写操作也不会影响到读指针. 下面是一个通用的实现代码示例: 

uart_com.c

```c
#include "uart_com.h"
#include "lwrb.h"

#define UART_COM_MAX (sizeof(uart_coms) / sizeof(uart_com_dma_t))

/* 将 UART 绑定到标准输入输出 */
#define UART_BIND_STDIO &huart1

typedef struct
{
    lwrb_t rb;                 /* 环形缓冲器 */
    uint8_t rbbuf[4096];       /* 环形缓冲器的 buffer */
    uint8_t dma_rxbuf[256];    /* DMA 接收 buffer */
    uint8_t dma_txbuf[4096];   /* DMA 发送 buffer */
    size_t old_pos;            /* 保存 DMA 数据旧的位置信息 */
    UART_HandleTypeDef *huart; /* HAL 库串口句柄 */
    uint8_t dma_tx_ready;      /* 用于检测串口是否可以发送数据 */
} uart_com_dma_t;

/**
 * @brief 定义 uart_com 数组
 *
 * @note 对于新增的串口, 比如 huart2
 *      只需往表中添加 { .huart = &huart2 } 即可，
 *      然后就可以使用 uart_write 和 uart_read_from_rb 进行通信了
 */
static uart_com_dma_t uart_coms[] = {
    {.huart = &huart1}
};

static inline uart_com_dma_t *get_uart_com(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < UART_COM_MAX; i++)
    {
        if (uart_coms[i].huart == huart)
            return uart_coms + i;
    }

    return NULL;
}

/* 接收处理 */
static void uart_rx_check(UART_HandleTypeDef *huart)
{
    uart_com_dma_t *c = get_uart_com(huart);
    if (c == NULL)
        return;

    /* 计算当前存储位置 */
    size_t pos = sizeof(c->dma_rxbuf) - __HAL_DMA_GET_COUNTER(huart->hdmarx);
    if (pos > c->old_pos)
    {
        lwrb_write(&(c->rb), c->dma_rxbuf + c->old_pos, pos - c->old_pos);
        c->old_pos = pos;
    }

    else if (pos < c->old_pos)
    {
        lwrb_write(&(c->rb), c->dma_rxbuf + c->old_pos, sizeof(c->dma_rxbuf) - c->old_pos);
        if (pos > 0)
            lwrb_write(&(c->rb), c->dma_rxbuf, pos);
        c->old_pos = pos;
    }
}

/**
 * @brief 串口初始化
 *
 * 该函数将所有声明在数组里的 uart_com 进行初始化,
 * 所有串口都初始化为:
 *      1. 已经准备好发送了
 *      2. 开 IDLE 中断
 *      2. 开始 DMA 循坏接收
 *
 * 循环接收的数据将通过中断自动写入环形缓冲区里面
 * 可通过 uart_read_from_rb 获取
 */
void uart_com_init(void)
{
    for (int i = 0; i < UART_COM_MAX; i++)
    {
        uart_com_dma_t *c = uart_coms + i;
        c->dma_tx_ready = 1;

        lwrb_init(&(c->rb), c->rbbuf, sizeof(c->rbbuf));

        /* 启动 IDLE 中断 */
        __HAL_UART_ENABLE_IT(c->huart, UART_IT_IDLE);
        __HAL_UART_CLEAR_IDLEFLAG(c->huart);

        /* 开始 UART DMA 接收 */
        HAL_UART_Receive_DMA(c->huart, c->dma_rxbuf, sizeof(c->dma_rxbuf));
    }
}

/**
 * @brief 将数据写入到串口, 阻塞调用
 *
 * @param huart 要写入的串口
 * @param dat 数据内容
 * @param len 数据长度
 * @param timeout 超时时间
 *
 * @return 实际写入的数量, 若 <= 0 则写入失败
 */
int uart_write(UART_HandleTypeDef *huart, const void *dat, size_t len, int timeout)
{
    uart_com_dma_t *c = get_uart_com(huart);
    if (c == NULL || len > sizeof(c->dma_txbuf))
        return -1;

    uint32_t tick = HAL_GetTick();

    /* 等待上一次发送完成 */
    while (c->dma_tx_ready == 0 && (HAL_GetTick() - tick) < timeout)
        ;

    if (c->dma_tx_ready)
    {
        int min = len > sizeof(c->dma_txbuf) ? sizeof(c->dma_txbuf) : len;
        memcpy(c->dma_txbuf, dat, min);
        HAL_UART_Transmit_DMA(c->huart, c->dma_txbuf, min);
        c->dma_tx_ready = 0;
        return min;
    }

    /* 等待超时 */
    return 0;
}

/**
 * @brief 从串口缓冲区读取数据
 *
 * @param huart 要读取的串口
 * @param dat 读取的数据
 * @param len 期望读取数据的数量
 *
 * @return 实际读取的数据数量, 若 <= 0 则读取失败
 */
int uart_read_from_rb(UART_HandleTypeDef *huart, uint8_t *dat, size_t len)
{
    uart_com_dma_t *c = get_uart_com(huart);
    if (c == NULL)
        return -1;

    return lwrb_read(&(c->rb), dat, len);
}

/* -------------------- 处理串口中断 -------------------------- */

/* 发送完成中断 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    uart_com_dma_t *c = get_uart_com(huart);
    if (c)
        c->dma_tx_ready = 1;
}

/* 接收半满中断 */
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    uart_rx_check(huart);
}

/* 接收完成中断 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uart_rx_check(huart);
}

/* IDLE 中断, 需放到 USARTx_IRQHandler 中断入口函数中执行 */
void UART_IDLE_Callback(UART_HandleTypeDef *huart)
{
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET)
    {
        uart_rx_check(huart);
        __HAL_UART_CLEAR_IDLEFLAG(huart);
    }
}

/* -------------------- 将串口绑定到标准输入输出 -------------------------- */

#ifdef UART_BIND_STDIO
#include <stdio.h>

int fputc(int ch, FILE *fd)
{
    uint8_t dat = ch;
    if (fd == stdout)
        HAL_UART_Transmit(UART_BIND_STDIO, &dat, 1, 50);

    return ch;
}

int fgetc(FILE *fd)
{
    uint8_t ch;
    while (fd == stdin && uart_read_from_rb(UART_BIND_STDIO, &ch, 1) <= 0)
        ;

    return ch;
}

/**
 * 如果不是使用 MicroLib 则需告知编译器不使用半主机模式
 * 以避免编译器默认使用半主机模式, 导致程序卡在初始化阶段无法退出
 */
#ifndef __MICROLIB

#if __ARMCC_VERSION >= 6000000
__asm(".global __use_no_semihosting");
#elif __ARMCC_VERSION >= 5000000
#pragma import(__use_no_semihosting)
#else
#error "Unsupported compiler"
#endif

/* 使用标准库但不使用半主机模式, 就需要自己实现一些标准库所依赖的系统函数 */

#include <rt_misc.h>
#include <rt_sys.h>
#include <time.h>

const char __stdin_name[] = ":tt";
const char __stdout_name[] = ":tt";
const char __stderr_name[] = ":tt";

FILEHANDLE _sys_open(const char *name, int openmode)
{
    return 1;
}

int _sys_close(FILEHANDLE fh)
{
    return 0;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}

int _sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode)
{
    return 0;
}

int _sys_read(FILEHANDLE fh, unsigned char *buf, unsigned len, int mode)
{
    return -1;
}

void _ttywrch(int ch)
{
}

int _sys_istty(FILEHANDLE fh)
{
    return 0;
}

int _sys_seek(FILEHANDLE fh, long pos)
{
    return -1;
}

long _sys_flen(FILEHANDLE fh)
{
    return -1;
}

void _sys_exit(int return_code)
{
    while (1)
        ;
}

clock_t clock(void)
{
    clock_t tmp;
    return tmp;
}

void _clock_init(void)
{
}

time_t time(time_t *timer)
{
    time_t tmp;
    return tmp;
}

int system(const char *string)
{
    return 0;
}

char *getenv(const char *name)
{
    return NULL;
}

void _getenv_init(void)
{
}
#endif /* __MICROLIB */

#endif
```

我们在程序中使能了空闲 (IDLE) 中断, 目的让串口能够在接收到任意字符后立刻做出反应. 

因为如果不使用空闲中断, 那么程序将一直等到 DMA 接收半满或全满才响应, 这样的话如果用户只发几个字符, 我们的程序将无法及时处理.

通过将串口绑定到标准输入输出后, 我们可以直接使用 stdio 的 scanf 和 printf 来进行输入输出:
```c
#include <stdio.h>
#include "uart_com.h"

void app_main(void)
{
    uart_com_init();

    printf("Hello World\n");

    while (1)
    {
        int aa;
        scanf("%d", &aa);
        printf("Your input is: %d\n", aa);
    }
}
```

### 参考文献

程序中使用到了开源的环形缓冲器的实现 [lwrb](https://github.com/MaJerle/lwrb.git)

另外, 禁用半主机模式的部分参考了 [Disable semihosting with ARM Compiler 5/6](https://mklimenko.github.io/english/2018/11/02/disable-semihosting/)

是否使用 MicroLib 的宏 `__MICROLIB` 参考了 ARM 官方源码里面的 [ARM-software/Tool-Solutions](https://github.com/ARM-software/Tool-Solutions/blob/main/docker/cmsis-models/Platforms/IPSS/ARMCM7/system_ARMCM7.c)


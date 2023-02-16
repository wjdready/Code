#ifndef __UART_COM_H__
#define __UART_COM_H__

#include "usart.h"


/* -- function prototypes -- */

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
void uart_com_init(void);

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
int uart_write(UART_HandleTypeDef *huart, const void *dat, size_t len, int timeout);

/**
 * @brief 从串口缓冲区读取数据
 *
 * @param huart 要读取的串口
 * @param dat 读取的数据
 * @param len 期望读取数据的数量
 *
 * @return 实际读取的数据数量, 若 <= 0 则读取失败
 */
int uart_read_from_rb(UART_HandleTypeDef *huart, uint8_t *dat, size_t len);

/* -- END OF function prototypes -- */

#endif


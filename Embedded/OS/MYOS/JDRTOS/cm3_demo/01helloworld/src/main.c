#include <stdio.h>
#include <stdint.h>

volatile uint32_t *const UART0DR = (uint32_t *)0x4000C000;

void uart0_send_str(char *s)
{
    while (*s != '\0')
        *UART0DR = *s++;
}

float pi = 3.1415926;

int main(int argc, char const *argv[])
{
    /* 使用半主机调试 */
    extern void initialise_monitor_handles(void);
    initialise_monitor_handles();
    printf("Hello World pi = %f\n", pi);

    uart0_send_str("Send from uart\n");
    
    /* 未定义指令, 产生一个 hardfault */
    asm("udf #1");

    return 0;
}

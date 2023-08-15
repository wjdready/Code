#include <stdio.h>
#include <stdint.h>
#include "jdrtos.h"
#include "cm3.h"

volatile uint32_t *const UART0DR = (uint32_t *)0x4000C000;

void uart0_send_str(char *s)
{
    while (*s != '\0')
        *UART0DR = *s++;
}

void thread_module_init(void);

int main(int argc, char const *argv[])
{
    /* 使用半主机调试 */
    extern void initialise_monitor_handles(void);
    initialise_monitor_handles();

    uart0_send_str("JDRTOS\n");
    printk("Hello RTOS\n");
    // printk("psp:0x%x\n", get_psp());
    // printk("msp:0x%x\n", get_msp());

    thread_module_init();

    sys_init();

    return 0;
}

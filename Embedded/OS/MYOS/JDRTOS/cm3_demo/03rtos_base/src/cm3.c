#include "cm3.h"
#include "jdrtos.h"

void init_systick(void)
{
    systick_t *systick_p = (systick_t *)SYSTICK_BASE;
    uint8_t *sys_prio_p = (uint8_t *)SYSTICK_PRIO_REG;
    *sys_prio_p = 0xf0;
    systick_p->load = (HSI_CLK & 0xffffffUL) - 1;
    systick_p->val = 0;
    systick_p->ctrl = 0x7;
}

void sys_init(void)
{
    init_systick();

    set_psp(0);
    MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
    MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}

void thread_tick_handler(void);

void systick_handler(void)
{
    // printk("systick_handler\n");
    thread_tick_handler();
}



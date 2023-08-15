#include "cm3.h"
#include "jdrtos.h"

#define SCS_BASE (0xE000E000)            /* System Control Space Base Address */
#define SYSTICK_BASE (SCS_BASE + 0x0010) /* SysTick Base Address*/
#define SCB_BASE (SCS_BASE + 0x0D00)
#define HSI_CLK 12000000UL
#define SYSTICK_PRIO_REG (0xE000ED23)

void init_systick(void)
{
    systick_t *systick_p = (systick_t *)SYSTICK_BASE;
    uint8_t *sys_prio_p = (uint8_t *)SYSTICK_PRIO_REG;
    *sys_prio_p = 0xf0;
    systick_p->load = (HSI_CLK & 0xffffffUL) - 1;
    systick_p->val = 0;
    systick_p->ctrl = 0x7;
}

void systick_handler(void)
{
    printk("systick_handler\n");
}

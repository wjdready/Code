#ifndef CM3_H
#define CM3_H
#include <stdint.h>

typedef struct systick_tag
{
    volatile uint32_t ctrl;
    volatile uint32_t load;
    volatile uint32_t val;
    volatile uint32_t calrb;
} systick_t;

#define SCS_BASE (0xE000E000)            /* System Control Space Base Address */
#define SYSTICK_BASE (SCS_BASE + 0x0010) /* SysTick Base Address*/
#define SCB_BASE (SCS_BASE + 0x0D00)
#define HSI_CLK 12000000UL
#define SYSTICK_PRIO_REG (0xE000ED23)

#define NVIC_INT_CTRL       0xE000ED04
#define NVIC_PENDSVSET      0x10000000
#define NVIC_SYSPRI2        0xE000ED22
#define NVIC_PENDSV_PRI     0x000000FF

#define MEM32(addr)         *(volatile uint32_t *)(addr)
#define MEM8(addr)          *(volatile uint8_t *)(addr)

extern uint32_t get_psp(void);
extern uint32_t get_msp(void);
extern uint32_t get_control_reg(void);
extern void set_psp(uint32_t sp);
void sys_init(void);

#endif

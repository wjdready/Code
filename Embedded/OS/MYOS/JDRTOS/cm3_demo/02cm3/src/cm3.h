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

extern uint32_t get_psp(void);
extern uint32_t get_msp(void);
extern uint32_t get_control_reg(void);

extern void init_systick(void);

#endif

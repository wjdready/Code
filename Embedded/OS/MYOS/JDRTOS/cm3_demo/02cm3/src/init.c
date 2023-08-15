#include <stdint.h>

void hardfault_handler(void)
{
    asm("mov r0, lr");
    asm("mov r1, sp");
    asm("bl hardfault_backtrace");
    while (1)
        ;
}

void init_bss_section(void)
{
    extern uint32_t _bss_start;
    extern uint32_t _bss_end;
    for (uint32_t *p = &_bss_start; p < &_bss_end;)
        *p++ = 0;
}

void init_data_section(void)
{
    extern uint32_t _data_start;
    extern uint32_t _data_end;
    extern uint32_t _init_data_start;
    uint32_t *d = &_init_data_start;
    for (uint32_t *p = &_data_start; p < &_data_end;)
        *p++ = *d++;
}

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* 检查对应地址处是否是 BL, BLX */
static int check_ins(uint32_t addr)
{

#define BL_INS_MASK 0xF800
#define BL_INS_HIGH 0xF800
#define BL_INS_LOW 0xF000
#define BLX_INX_MASK 0xFF00
#define BLX_INX 0x4700

    uint16_t ins1 = *((uint16_t *)addr);
    uint16_t ins2 = *((uint16_t *)(addr + 2));

    if ((ins2 & BL_INS_MASK) == BL_INS_HIGH && (ins1 & BL_INS_MASK) == BL_INS_LOW)
        return 1;
    else if ((ins2 & BLX_INX_MASK) == BLX_INX)
        return 1;
    else
        return 0;
}

typedef struct
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
} fault_reg_t;

/* 根据栈回溯 */
static void backtrace(uint32_t pc, uint32_t sp)
{
    extern uint32_t _estack;
    extern uint32_t _code_start;
    extern uint32_t _code_end;

    uint32_t code_start = (uint32_t)(uint32_t *)&_code_start;
    uint32_t code_end = (uint32_t)(uint32_t *)&_code_end;
    uint32_t stack_top = (uint32_t)(uint32_t *)(&_estack);

    printf("backtrace: %08X ", pc);

    for (; sp < stack_top; sp += sizeof(size_t))
    {
        pc = *((uint32_t *)sp) - sizeof(size_t);
        if (pc % 2 == 0)
            continue;

        pc = *((uint32_t *)sp) - 1;
        if (pc < code_start || pc >= code_end)
            continue;

        int r = check_ins(pc - sizeof(size_t));
        if (!r)
            continue;

        // 获取 pc 值
        printf("%08X ", pc);
    }

    printf("\n");
}

void hardfault_backtrace(uint32_t lr, uint32_t sp)
{
    fault_reg_t *reg = (fault_reg_t *)sp;
    backtrace(reg->pc, sp);
    while (1)
        ;
}

void softfault_backtrace(void)
{
    uint32_t pc, sp;
    asm("mov %0, lr" : "=r"(pc));
    asm("mov %0, sp" : "=r"(sp));
    backtrace(pc, sp);
    while (1)
        ;
}

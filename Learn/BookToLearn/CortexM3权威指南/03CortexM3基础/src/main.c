#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

volatile uint32_t *const UART0DR = (uint32_t *)0x4000C000;

const int a = 123; // 常量在编译阶段就已经被设为固定值了
int b = 456;       // 全局变量如果没有加载阶段的赋值，运行时其值不确定

void send_str(char *s)
{
    int c = 789; // 局部变量不需要初始化值, 仅仅依赖栈的正确设置即可使用

    while (*s != '\0')
        *UART0DR = *s++;
}

char send_char(uint8_t *ch)
{
    *UART0DR = *ch;
    return *ch;
}

int printf_char(char ch)
{
    uint8_t c1 = (uint8_t)'\r';
    uint8_t c2 = 0;

    c2 = (uint8_t)ch;

    send_char(&c2);

    if (((uint8_t)'\n') == c2) {
        send_char(&c1);
    }

    return 0;
}

int printf_str(char *str)
{
    while (str && (*str != (char)'\0')) {
        printf_char(*str);
        str++;
    }

    return 0;
}

const char hex_asc_table[16] =
    { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
int printf_hex(uint32_t val, uint32_t width)
{
    int i = 0;
    char hex_val = 0, asc = 0;

    if ((width > 8) || (width == 0))
        width = 8;

    for (i = width - 1; i >= 0; i--) {
        hex_val = (val & (0x0F << (i << 2))) >> (i << 2);
        asc = hex_asc_table[(int)hex_val];
        printf_char(asc);
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    send_str("Hello World\n");

    return 0;
}

/* check the disassembly instruction is 'BL' or 'BLX' */
static int disassembly_ins_is_bl_blx(uint32_t addr)
{
    uint16_t ins1 = *((uint16_t *)addr);
    uint16_t ins2 = *((uint16_t *)(addr + 2));

#define BL_INS_MASK 0xF800
#define BL_INS_HIGH 0xF800
#define BL_INS_LOW 0xF000
#define BLX_INX_MASK 0xFF00
#define BLX_INX 0x4700

    if ((ins2 & BL_INS_MASK) == BL_INS_HIGH && (ins1 & BL_INS_MASK) == BL_INS_LOW)
        return 1;
    else if ((ins2 & BLX_INX_MASK) == BLX_INX)
        return 1;
    else
        return 1;
}

// 在 ld 中定义的变量 比如 xxx = .; 其地址就是 . 所在的位置, 因此要获取 . 位置处的地址, 需要 int p = &xxx;
extern uint32_t _estack;
extern uint32_t _code_start;
extern uint32_t _code_end;

void backtrace(uint32_t lr, uint32_t sp)
{
    uint32_t *reg = (uint32_t *)sp;
    char buf[64] = "Good\n";

    send_str("pc = 0x");
    printf_hex(reg[6], 8);
    send_str("\n");

    uint32_t code_start = (uint32_t)(uint32_t *)&_code_start;
    uint32_t code_end = (uint32_t)(uint32_t *)&_code_end;
    uint32_t stack_top = (uint32_t)(uint32_t*)(&_estack);
    
    for (; sp < stack_top; sp += sizeof(size_t))
    {
        uint32_t pc = *((uint32_t *) sp) - sizeof(size_t);
        if (pc % 2 == 0)
            continue;

        pc = *((uint32_t *)sp) - 1;
        if (pc < code_start || pc >= code_end)
            continue;

        int r = disassembly_ins_is_bl_blx(pc - sizeof(size_t));
        if (!r)
            continue;
        
        int good = pc;
        send_str("0x");
        printf_hex(pc, 8);
        send_str("\n");
    }

    // Register R0
    // Register R1
    // Register R2
    // Register R3
    // Register R12
    // Link register LR
    // Program counter PC
    // Program status word PSR

    send_str(buf);
}

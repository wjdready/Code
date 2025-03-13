#include "stdint.h"

enum color
{
    BLACK = 0,
    BRIGHT = 7
};

enum size
{
    COLS = 80,
    ROWS = 25
};

uint16_t *const video = (uint16_t *)0xB8000;

#define PUTC(x, y, fg, bg, c) \
    video[(y) * COLS + (x)] = ((bg) << 12) | ((fg) << 8) | (c)


extern void loadbin(uint8_t sector, uint8_t number, uint16_t segaddr, uint16_t offset);

uint32_t bootaddr = (uint32_t)0x90000;

int __attribute__((noreturn)) main()
{
    int i = 0;
    const char *str = "Hello World";

loop:
    video[i] = 0x00;
    i ++;
    if (i < COLS * ROWS)
        goto loop;

    i = 0;

printStr:
    PUTC(i, 0, BRIGHT, BLACK, str[i]);
    i++;
    if (i < 10)
        goto printStr;

    // loadbin(1, 5, 0x9000, 0x00);

    // ((void (*)(void)) bootaddr)();

    for (int i = 0; i < 100; i++)
    {
        PUTC(i, 0, BRIGHT, BLACK, 'A');
    }
    
    while (1)
        ;
}

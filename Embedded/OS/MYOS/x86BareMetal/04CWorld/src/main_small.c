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

int __attribute__((noreturn)) main()
{
    int i = 0;
    const char *str = "Hello";

loop:
    video[i] = 0x00;
    i ++;
    if (i < COLS * ROWS)
        goto loop;

    i = 0;

printStr:
    PUTC(i, 0, BRIGHT, BLACK, str[i]);
    i++;
    if (i < 5)
        goto printStr;

    while (1)
        ;
}

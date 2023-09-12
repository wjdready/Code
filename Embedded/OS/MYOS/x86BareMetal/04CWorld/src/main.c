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

void putc(uint8_t x, uint8_t y, enum color fg, enum color bg, char c)
{
    video[y * COLS + x] = (bg << 12) | (fg << 8) | c;
}

void puts(uint8_t x, uint8_t y, enum color fg, enum color bg, const char *s)
{
    for (; *s; s++, x++)
        putc(x, y, fg, bg, *s);
}

int main(void)
{
    const char *s = "Hello World";

    // 512 Byte 代码空间, 只写一个 for 就快用没了
    for(int i = 0; i < 25; i++)
        puts(0, i, BRIGHT, BLACK, s);

    // int es_set = 0x9000, es_backup = 0;
    // asm 
    // (
    //     "mov %%es, %0 \n"
    //     "mov %1, %%es \n"
    //     :"=m"(es_backup)       
    //     :"r"(es_set)
    //     :"eax"
    // );

    // uint8_t *src = (uint8_t *)0x7c00;
    // uint8_t *dst = (uint8_t *)0x90000;
    // for (int i = 0; i < 512; i++) 
    //     dst[i] = src[i];

    while (1)
        ;
}

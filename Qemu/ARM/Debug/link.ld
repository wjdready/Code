/* 程序入口 */
ENTRY(reset)

/* 栈指针初始位置, 在这里定义的等式可在 */
/* _estack = 0x20020000; */

/* 存储器分布 */
/* MEMORY
{
    DTCMRAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 128K
    FLASH (rx)          : ORIGIN = 0x8000000, LENGTH = 128K
} */

/* 段组织 */
SECTIONS
{
    . = 0x60010000;

    /* 代码段紧随其后 */
    .text :
    {
        *(.vectors)
        *(.text)
        *(.text.*)
    }
}

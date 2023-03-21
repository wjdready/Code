/* 程序入口 */
ENTRY(reset)

/* 栈指针初始位置, 在这里定义的等式可在 */
_estack = 0x20020000;

/* 存储器分布 */
MEMORY
{
    DTCMRAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 128K
    FLASH (rx)          : ORIGIN = 0x8000000, LENGTH = 128K
}

/* 段组织 */
SECTIONS
{
    /* FLASH 起始地址放置中断向量表 */
    .my_vertor :
    {
        /* 中断向量表的数据一般是由CPU中断自动调用的, 在程序中有可能没被引用, 
        容易被GCC的优化程序当成垃圾回收, 因此使用 KEEP 保持原样不变 */
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH

    /* 代码段紧随其后 */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        . = ALIGN(4);
    } >FLASH
}

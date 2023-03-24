/* 程序入口 */
ENTRY(reset)

/* 栈指针初始位置 */
_estack = ORIGIN(RAM) + LENGTH(RAM);

/* 用于预估堆栈大小 */
_heap_size = 0x2000;
_stack_size = 0x4000;

/* 存储器分布 */
MEMORY
{
    ROM (rx)          : ORIGIN = 0x60010000, LENGTH = 128K
    RAM (xrw)           : ORIGIN = 0x60020000, LENGTH = 128K
}

/* 段组织 */
SECTIONS
{
    /* 代码 */
    .text :
    {
        KEEP(*(.vectors))
        *(.text)
        *(.text.*)

        KEEP (*(.init))
        KEEP (*(.fini))

    } >ROM

    /* 只读数据 */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
    } >ROM

    /* 将读写数据段放到 data 段 */
    .data : 
    {
        . = ALIGN(4);
        _data_start = .; 

        *(.data)
        *(.data*)

        . = ALIGN(4);
        _data_end = .;
    } >RAM AT> ROM

    /* 指向 flash 中用于初始化的 data */
    _init_data_start = LOADADDR(.data);

    /* 未初始化数据放到 bss 段 */
    .bss :
    {
        . = ALIGN(4);
        _bss_start = .;
        __bss_start__ = _bss_start;
        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);
        _bss_end = .;
        __bss_end__ = _bss_end;
    } >RAM

    /* 堆 */
    .heap :
    {
        . = ALIGN(8);
        _heap_start = .;
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _heap_size;
        _heap_end = .;
        . = . + _stack_size;
        . = ALIGN(8);
    } >RAM
}

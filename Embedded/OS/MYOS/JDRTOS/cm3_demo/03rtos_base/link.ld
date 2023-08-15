/* 程序入口 */
/* ENTRY(reset) */

/* 主栈指针初始位置 */
_estack = ORIGIN(DTCMRAM) + LENGTH(DTCMRAM);

/* 用于预估堆栈大小 */
_heap_size = 0x2000;
_stack_size = 0x4000;

__ROM_BASE = 0x00000000;
__ROM_SIZE = 0x00040000;

__RAM_BASE = 0x20000000;
__RAM_SIZE = 0x00010000;

/* 存储器分布 */
MEMORY
{
    DTCMRAM (xrw)       : ORIGIN = __RAM_BASE, LENGTH = __RAM_SIZE
    FLASH (rx)          : ORIGIN = __ROM_BASE, LENGTH = __ROM_SIZE
}

/* 段组织 */
SECTIONS
{
    /* 代码 */
    .text :
    {
        KEEP(*(.isr_vector))
        _code_start = .;
        *(.text)
        *(.text.*)

        KEEP (*(.init))
        KEEP (*(.fini))
        _code_end = .;
    } >FLASH

    /* 只读数据 */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
    } >FLASH

    /* 将读写数据段放到 data 段 */
    .data : 
    {
        . = ALIGN(4);
        _data_start = .; 

        *(.data)
        *(.data*)

        . = ALIGN(4);
        _data_end = .;
    } >DTCMRAM AT> FLASH

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
    } >DTCMRAM
    
    /* 之后的空间用来分配堆栈 */
    .heap :
    {
        . = ALIGN(8);
        _heap_start = .;
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _heap_size;
        _heap_end = .;
        . = . + _stack_size;
        _estack_psp = .;        /* 用户堆栈 */
        . = ALIGN(8);
    } >DTCMRAM

    /* 一些杂项 */
    .exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH
}

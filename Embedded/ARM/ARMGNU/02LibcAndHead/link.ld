/* 程序入口 */
ENTRY(reset)

/* 栈指针初始位置 */
_estack = ORIGIN(DTCMRAM) + LENGTH(DTCMRAM);

_Min_Heap_Size = 0x2000;  /* required amount of heap  */
_Min_Stack_Size = 0x4000; /* required amount of stack */

/* 存储器分布 */
MEMORY
{
    DTCMRAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 128K
    FLASH (rx)          : ORIGIN = 0x8000000, LENGTH = 128K
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
        _sdata = .;        /* 在 data 开头定义一个全局标签 _sdata 以便在汇编中访问 */

        *(.data)
        *(.data*)

        . = ALIGN(4);
        _edata = .;        /* 在 data 结尾定义一个全局标签 _edata 以便在汇编中访问 */
    } >DTCMRAM AT> FLASH

    .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
    .ARM : {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >FLASH

    .preinit_array     :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >FLASH
    .init_array :
    {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH
    .fini_array :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH

    /* used by the startup to initialize data */
    _sidata = LOADADDR(.data);

    /* 未初始化数据放到 bss 段 */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;         /* 在 bss 开头定义一个全局标签 _sbss 以便在汇编中访问 */
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);
        _ebss = .;         /* 在 bss 结尾定义一个全局标签 _ebss 以便在汇编中访问 */
        __bss_end__ = _ebss;
    } >DTCMRAM

    /* 堆 */
    .heap :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
    } >DTCMRAM
}

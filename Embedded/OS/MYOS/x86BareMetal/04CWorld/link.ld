
/* MEMORY
{
    BIOSRAM (rwx) : ORIGIN = 0x7c00, LENGTH = 0x200
} */

SECTIONS
{
    . = 0x7c00;
    /* . = 0; */
    .text : { *(.text) }
    .data : { *(.data) }
    .rodata : { *(.rodata) }

    . = 0x7dfe;
    .boot_end : {
        *(.boot_end)
    }
}

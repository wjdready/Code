
ENTRY(reset)

SECTIONS
{
    . = 0x60010000;

    .text :
    {
        *(.vectors)
        *(.text)
        *(.text.*)
    }

    end = .;
}


#include <stdint.h>

typedef struct efi_text
{
    void *a;
    int (*output)(struct efi_text *this, const wchar_t *str);
} efi_text_t;

typedef struct
{
    char a[52];
    void *console;
    efi_text_t *text;
} efi_sys_table_t;

int efi_main(void *img_handle, efi_sys_table_t *table)
{
    table->text->output(table->text, L"Hello EFI!");
    while (1)
        ;
    return 0;
}

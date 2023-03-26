#include "test_command.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 打印内存 */
void display_memory(uint8_t *buf, int len)
{
    uint8_t str[16];
    int j = 0;
    for (int i = 0; i < len; i++)
    {
        if (i % 16 == 0)
            printf("\n0x%04X: ", i);
        printf("%02X ", buf[i]);
        str[j++] = buf[i];
        if (j >= 16)
        {
            printf("\t");
            for (int x = 0; x < j; x++)
                printf("%c", isprint(str[x]) ? str[x] : '.');
            j = 0;
        }
    }

    if (j >= 0)
    {
        for (int i = 0; i < 16 - j; i++)
            printf("   ");

        printf("\t");

        for (int x = 0; x < j; x++)
            printf("%c", isprint(str[x]) ? str[x] : '.');
        j = 0;
    }

    printf("\n");
}

static int memory_read_test(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <addr> <size>\n", argv[0]);
        return -1;
    }

    uint32_t addr = strtoul(argv[1], NULL, 16);
    int size = atoi(argv[2]);

    printf("read addr:0x%08X, size: %d bytes\n", addr, size);

    // 使用缓冲区
    // static uint8_t buf[1024];
    // size > 1024 ? size = 1024 : 1;
    // memset(buf, 0, sizeof(buf));
    // memcpy(buf, (void*)addr, size);

    display_memory((uint8_t *)addr, size);

    return 0;
}
EXPORT_TEST_COMMAND(memory_read_test, "read", "读取内存");

static int memory_write_test(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <addr> <data1 data2 ... datan>\n", argv[0]);
        return -1;
    }

    uint32_t addr = strtoul(argv[1], NULL, 16);
    int size = atoi(argv[2]);

    size > 1024 ? size = 1024 : 1;

    static uint8_t buf[1024];

    int ofs = 0;
    for (int i = 2; i < argc; i++)
    {
        char *p = argv[i];
        while (*p && ofs < (sizeof(buf) - 1))
            buf[ofs++] = *p++;
        if (i != (argc - 1))
            buf[ofs++] = ' ';
    }

    buf[ofs] = '\0';

    printf("write addr: 0x%08X, data: %s, size: %d bytes\n", addr, buf, ofs);

    memcpy((uint8_t *)addr, buf, ofs);

    return 0;
}
EXPORT_TEST_COMMAND(memory_write_test, "write", "写入内存");

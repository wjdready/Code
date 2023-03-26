/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "hardware/flash.h"
#include "pico/stdlib.h"

// We're going to erase and reprogram a region 256k from the start of flash.
// Once done, we can access this at XIP_BASE + 256k.
#define FLASH_TARGET_OFFSET (256 * 1024)

const uint8_t *flash_target_contents = (const uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);

void print_buf(const uint8_t *buf, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        printf("%02x", buf[i]);
        if (i % 16 == 15)
            printf("\n");
        else
            printf(" ");
    }
}

#include "test_command.h"

void test_command_handler(void)
{
    static char input_string[1024];

    gets(input_string);

    test_command(input_string);

    printf("\n#sh ");
}

int main()
{
    stdio_init_all();
    // uint8_t random_data[FLASH_PAGE_SIZE];
    // for (int i = 0; i < FLASH_PAGE_SIZE; ++i)
    //     random_data[i] = rand() >> 16;

    // printf("Generated random data:\n");
    // print_buf(random_data, FLASH_PAGE_SIZE);

    // // Note that a whole number of sectors must be erased at a time.
    // printf("\nErasing target region...\n");
    // flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
    // printf("Done. Read back target region:\n");
    // print_buf(flash_target_contents, FLASH_PAGE_SIZE);

    // printf("\nProgramming target region...\n");
    // flash_range_program(FLASH_TARGET_OFFSET, random_data, FLASH_PAGE_SIZE);
    // printf("Done. Read back target region:\n");
    // print_buf(flash_target_contents, FLASH_PAGE_SIZE);

    // bool mismatch = false;
    // for (int i = 0; i < FLASH_PAGE_SIZE; ++i) {
    //     if (random_data[i] != flash_target_contents[i])
    //         mismatch = true;
    // }
    // if (mismatch)
    //     printf("Programming failed!\n");
    // else
    //     printf("Programming successful!\n");

    printf("\n#sh ");

    while (1)
    {
        test_command_handler();
    }

    // char buf[256];
    // gets(buf);

    // printf("%s\n", buf);
    // display_memory(0x00, 1024);
}

#define PICO_DEFAULT_LED_PIN 4

#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdio.h>

int duty = 75;
int freq = 75;

/* __TEST_COMMAND__START__ */
#include "test_command.h"
static int pwdd(int argc, char *argv[])
{
    printf("Hello World\n");

    if (argc != 2)
    {
        return;
    }

    freq = atoi(argv[1]);

    printf("freq %d\n", freq);

    return 0;
}
EXPORT_TEST_COMMAND(pwdd, "pf", "desc");
/* __TEST_COMMAND__END__ */

#include "test_command.h"
static int pwmf(int argc, char *argv[])
{
    printf("Hello World\n");

    if (argc != 2)
    {
        return;
    }

    duty = atoi(argv[1]);

    printf("duty %d\n", duty);

    return 0;
}
EXPORT_TEST_COMMAND(pwmf, "pd", "desc");
/* __TEST_COMMAND__END__ */


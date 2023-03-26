/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "pico/stdlib.h"
#include "hardware/flash.h"

// We're going to erase and reprogram a region 256k from the start of flash.
// Once done, we can access this at XIP_BASE + 256k.
#define FLASH_TARGET_OFFSET (256 * 1024)

const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);

void print_buf(const uint8_t *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
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

int main() {
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

#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

int duty = 75;
int freq = 75;


/* __TEST_COMMAND__START__ */
#include "test_command.h"
static int pwdd(int argc, char *argv[])
{
    printf("Hello World\n");
    
    if(argc != 2)
    {
        return ;
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
    
    if(argc != 2)
    {
        return ;
    }

    duty = atoi(argv[1]);

    printf("duty %d\n", duty);

    return 0;
}
EXPORT_TEST_COMMAND(pwmf, "pd", "desc");
/* __TEST_COMMAND__END__ */



#ifdef PICO_DEFAULT_LED_PIN
void on_pwm_wrap() {
    static int fade = 0;
    static bool going_up = true;
    // Clear the interrupt flag that brought us here
    pwm_clear_irq(pwm_gpio_to_slice_num(PICO_DEFAULT_LED_PIN));

    // if (going_up) {
    //     ++fade;
    //     if (fade > 255) {
    //         fade = 255;
    //         going_up = false;
    //     }
    // } else {
    //     --fade;
    //     if (fade < 0) {
    //         fade = 0;
    //         going_up = true;
    //     }
    // }
    // Square the fade value to make the LED's brightness appear more linear
    // Note this range matches with the wrap value
    uint slice_num = pwm_gpio_to_slice_num(PICO_DEFAULT_LED_PIN);
    
    pwm_set_gpio_level(PICO_DEFAULT_LED_PIN, duty);

    pwm_set_clkdiv(slice_num, freq);  // 将时钟频率设置为 2kHz
}
#endif
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
int aaaaa() {
#ifndef PICO_DEFAULT_LED_PIN
#warning pwm/led_fade example requires a board with a regular LED
#else
    // Tell the LED pin that the PWM is in charge of its value.
    gpio_set_function(PICO_DEFAULT_LED_PIN, GPIO_FUNC_PWM);
    // Figure out which slice we just connected to the LED pin
    uint slice_num = pwm_gpio_to_slice_num(PICO_DEFAULT_LED_PIN);

    // Mask our slice's IRQ output into the PWM block's single interrupt line,
    // and register our interrupt handler
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // Get some sensible defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 4.f);
    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);

    // Everything after this point happens in the PWM interrupt handler, so we
    // can twiddle our thumbs
    // while (1)
    //     tight_loop_contents();
#endif
}

/* __TEST_COMMAND__START__ */
#include "test_command.h"
static int HASHDKSAD(int argc, char *argv[])
{
    printf("Hello World\n");
    aaaaa();

    return 0;
}
EXPORT_TEST_COMMAND(HASHDKSAD, "pwm", "desc");
/* __TEST_COMMAND__END__ */
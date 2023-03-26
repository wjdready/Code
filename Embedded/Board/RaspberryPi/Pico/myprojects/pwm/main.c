#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "test_command.h"
#include <stdlib.h>

void test_command_handler(void)
{
    static char input_string[1024];
    int test_command(char *input_string);

    gets(input_string);

    test_command(input_string);

    printf("\n#sh ");
}

void my_pwm_init(void);

int main()
{
    stdio_init_all();

    // my_pwm_init();

    sleep_ms(2000);                             /// Just to wait some..
    gpio_set_function(4, GPIO_FUNC_PWM);  /// Set the pin 0 to be PWM
    auto slice   = pwm_gpio_to_slice_num(4);
    auto channel = pwm_gpio_to_channel(4);

    pwm_set_clkdiv(slice, 256.0f);  /// Setting the divider to slow down the clock
    pwm_set_wrap(slice, 9804);      /// setting the Wrap time to 9764 (20 ms)
    pwm_set_enabled(slice, true);

    for (uint i = 0; i < 10; ++i)
    {

        pwm_set_chan_level(slice, channel, 490);  /// Setting the duty period (0.6 ms)
        sleep_ms(2000);
        pwm_set_chan_level(slice, channel, 4092);  /// Setting the duty period (1.5 ms)
        sleep_ms(2000);
        pwm_set_chan_level(slice, channel, 9800);  /// Setting the duty period (2.4 ms)
        sleep_ms(2000);
    }

    printf("\n#sh ");

    while (1)
    {
        test_command_handler();
    }
}


#define CONFIG_PWM_PIN 4

int duty;
int freq;

void on_pwm_wrap(void)
{
    uint slice_num = pwm_gpio_to_slice_num(CONFIG_PWM_PIN);
    
    // Clear the interrupt flag that brought us here
    pwm_clear_irq(slice_num);

    // set duty
    // pwm_set_gpio_level(CONFIG_PWM_PIN, duty);
    // pwm_set_clkdiv(slice_num, freq);
}

void my_pwm_init(void)
{
    gpio_set_function(CONFIG_PWM_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(CONFIG_PWM_PIN);

    // Mask our slice's IRQ output into the PWM block's single interrupt line,
    // and register our interrupt handler
    pwm_clear_irq(slice_num);
    pwm_set_irq_enabled(slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    pwm_config config = pwm_get_default_config();

    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 4.f);
    pwm_set_gpio_level(CONFIG_PWM_PIN, 12345557);
    pwm_init(slice_num, &config, true);
}


static int pwm_set_freq(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <freq>\n", argv[0]);
        return -1;
    }

    freq = atoi(argv[1]);

    uint slice_num = pwm_gpio_to_slice_num(CONFIG_PWM_PIN);
    pwm_set_clkdiv(slice_num, freq);

    return 0;
}
EXPORT_TEST_COMMAND(pwm_set_freq, "pwm_set_freq", "设置 PWM 频率");


static int pwm_set_duty(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <duty>\n", argv[0]);
        return -1;
    }

    duty = atoi(argv[1]);

    pwm_set_gpio_level(CONFIG_PWM_PIN, duty);

    return 0;
}
EXPORT_TEST_COMMAND(pwm_set_duty, "pwm_set_duty", "设置 PWM 频率");



static int pwm_set_freq_fade(int argc, char *argv[])
{

    uint slice_num = pwm_gpio_to_slice_num(CONFIG_PWM_PIN);

    float f = 0.1f;

    while (f < 30.0f)
    {
        f += 1.f;
        pwm_set_clkdiv(slice_num, f);

        printf("%.2f\n", f);

        sleep_ms(100);
    }
    
    return 0;
}
EXPORT_TEST_COMMAND(pwm_set_freq_fade, "pwm_set_freq_fade", "设置 PWM 频率渐变");


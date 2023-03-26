
#define CONFIG_PWM_PIN 4

void on_pwm_wrap(void)
{
    uint slice_num = pwm_gpio_to_slice_num(CONFIG_PWM_PIN);
    
    // Clear the interrupt flag that brought us here
    pwm_clear_irq(slice_num);

    // set duty
    pwm_set_gpio_level(PICO_DEFAULT_LED_PIN, duty);

    pwm_set_clkdiv(slice_num, freq);
}



void pwm_init(void)
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
    pwm_init(slice_num, &config, true);
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
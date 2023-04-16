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

typedef uint8_t byte;

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

    // for (uint i = 0; i < 10; ++i)
    // {

    //     pwm_set_chan_level(slice, channel, 490);  /// Setting the duty period (0.6 ms)
    //     sleep_ms(2000);
    //     pwm_set_chan_level(slice, channel, 4092);  /// Setting the duty period (1.5 ms)
    //     sleep_ms(2000);
    //     pwm_set_chan_level(slice, channel, 9800);  /// Setting the duty period (2.4 ms)
    //     sleep_ms(2000);
    // }

    for (int i = 2; i < 1000000; i += 2)
    {
        pwm_set_wrap(slice, i);
        pwm_set_chan_level(slice, channel, i / 2);
        sleep_ms(1);
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
    pwm_config_set_clkdiv(&config, 1.0f);
    pwm_set_gpio_level(CONFIG_PWM_PIN, 12345557);
    pwm_init(slice_num, &config, true);
}


#include "hardware/clocks.h"

void set_pwm_freq(int freq) 
{
    printf("freq %d\n", freq);

    uint32_t pwm_clkfreq = 125000000; // PWM clock frequency
    uint32_t pwm_clkdiv = 1; // PWM clock divider

    uint32_t div = pwm_clkfreq / freq / 2; // calculate the PWM clock divider
    pwm_clkdiv = div + 1;
    
    printf("pwm_clkdiv %d\n", pwm_clkdiv);

    // set the PWM clock divider
    uint slice_num = pwm_gpio_to_slice_num(CONFIG_PWM_PIN); // map the GPIO pin to the corresponding PWM slice number
    pwm_set_clkdiv(slice_num, pwm_clkdiv);
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

void Beep(uint16_t freq, int delay)
{
    uint slice_num = pwm_gpio_to_slice_num(CONFIG_PWM_PIN);

    set_pwm_freq(freq);

    sleep_ms(delay);
}

const byte score[] = {0x90, 64, 1, 96,  0x90, 66, 1, 96,  0x90, 67, 4, 34,  0x90, 66, 1, 96,  0x90, 67,  2, 193,
                      0x90, 71, 2, 193, 0x90, 66, 8, 69,  0x90, 59, 2, 193, 0x90, 64, 4, 34,  0x90, 62,  1, 96,
                      0x90, 64, 2, 193, 0x90, 67, 2, 193, 0x90, 62, 8, 69,  0x90, 59, 2, 193, 0x90, 60,  4, 34,
                      0x90, 59, 1, 96,  0x90, 60, 1, 96,  0x90, 67, 4, 34,  0x90, 59, 4, 34,  0x90, 58,  1, 96,
                      0x90, 59, 1, 96,  0x90, 67, 2, 193, 0x90, 66, 4, 34,  0x90, 61, 1, 96,  0x90, 61,  2, 193,
                      0x90, 66, 2, 193, 0x90, 66, 8, 69,  0x90, 76, 1, 96,  0x90, 78, 1, 96,  0x90, 79,  4, 34,
                      0x90, 78, 1, 96,  0x90, 79, 2, 193, 0x90, 83, 2, 193, 0x90, 78, 8, 69,  0x90, 71,  2, 193,
                      0x90, 76, 4, 34,  0x90, 74, 1, 96,  0x90, 76, 2, 193, 0x90, 79, 2, 193, 0x90, 74,  8, 69,
                      0x90, 71, 2, 193, 0x90, 72, 2, 193, 0x90, 79, 1, 96,  0x90, 78, 2, 193, 0x90, 79,  2, 193,
                      0x90, 81, 2, 193, 0x90, 83, 1, 96,  0x90, 79, 4, 34,  0x90, 79, 1, 96,  0x90, 78,  1, 96,
                      0x90, 76, 2, 193, 0x90, 78, 2, 193, 0x90, 75, 2, 193, 0x90, 76, 8, 69,  0x80, 0xf0};

// Table of midi note frequencies * 2
//   They are times 2 for greater accuracy, yet still fits in a word.
//   Generated from Excel by =ROUND(2*440/32*(2^((x-9)/12)),0) for 0<x<128
// The lowest notes might not work, depending on the Arduino clock frequency
// Ref: http://www.phy.mtu.edu/~suits/notefreqs.html

#include <stdint.h>

const uint8_t _midi_byte_note_frequencies[48] = {
    16, 17, 18, 19, 21, 22, 23, 24, 26,  28,  29,  31,  33,  35,  37,  39,  41,  44,  46,  49,  52,  55,  58,  62,
    65, 69, 73, 78, 82, 87, 92, 98, 104, 110, 117, 123, 131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247};

const unsigned int _midi_word_note_frequencies[80] = {
    262,   277,   294,   311,   330,   349,   370,   392,   415,   440,   466,   494,   523,   554,   587,   622,
    659,   698,   740,   784,   831,   880,   932,   988,   1047,  1109,  1175,  1245,  1319,  1397,  1480,  1568,
    1661,  1760,  1865,  1976,  2093,  2217,  2349,  2489,  2637,  2794,  2960,  3136,  3322,  3520,  3729,  3951,
    4186,  4435,  4699,  4978,  5274,  5588,  5920,  6272,  6645,  7040,  7459,  7902,  8372,  8870,  9397,  9956,
    10548, 11175, 11840, 12544, 13290, 14080, 14917, 15804, 16744, 17740, 18795, 19912, 21096, 22351, 23680, 25088};

// const byte score[] = {0x90, 83, 0, 75, 0x80, 0x90, 88, 0, 225, 0x80, 0xf0};

#define TUNE_OP_PLAYNOTE 0x90 /* play a note: low nibble is generator #, note is next byte */
#define TUNE_OP_STOPNOTE 0x80 /* stop a note: low nibble is generator # */
#define TUNE_OP_RESTART 0xe0  /* restart the score from the beginning */
#define TUNE_OP_STOP 0xf0     /* stop playing */

unsigned duration;

void playNote(uint8_t ch, uint8_t note)
{
    if (note > 127)
    {
        return;
    }

    if (ch != 0)
    {
        return;
    }

    printf("note %d duration %d\n", note, duration);
    int frequency2;

    if (note < 48)
    {
        frequency2 = *(_midi_byte_note_frequencies + note);
    }
    else
    {
        frequency2 = *(_midi_word_note_frequencies + note - 48);
    }

    
    Beep(frequency2, duration);
}

void playScore()
{
    int index = 0;
    int arraySize = sizeof(score) / sizeof(score[0]);
    uint8_t tone = 0;

    while (index < arraySize)
    {

        byte command, opcode, chan;
        // unsigned duration;

        command = *(score + index++);
        opcode = command & 0xf0;
        chan = command & 0x0f;

        if (opcode == TUNE_OP_STOPNOTE)
        { /* stop note */
            // stopNote(chan);
        }

        else if (opcode == TUNE_OP_PLAYNOTE)
        { /* play note */
            // all_muted = !outputEnabled();
            // playNote(chan, *(score + index++));
            tone = *(score + index++);
        }

        else if (opcode < 0x80)
        { /* wait count in msec. */
            duration = ((unsigned)command << 8) | *(score + index++);

            playNote(0, tone);

            //     wait_toggle_count = ((unsigned long)wait_timer_frequency2 * duration + 500) / 1000;
            //     if (wait_toggle_count == 0)
            //         wait_toggle_count = 1;
            //     break;
        }
    }
}


/* __TEST_COMMAND__START__ */
#include "test_command.h"
static int play(int argc, char *argv[])
{
    // printf("Hello World\n");
    playScore();
    return 0;
}
EXPORT_TEST_COMMAND(play, "play", "播放");
/* __TEST_COMMAND__END__ */
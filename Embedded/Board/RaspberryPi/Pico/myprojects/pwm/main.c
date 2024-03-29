#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "test_command.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CONFIG_PWM_PIN 9
#define PWM_COUNTER_MAX 1250
#define PWM_CLOCK 125000000.f

void test_command_handler(void)
{
    static char input_string[1024];
    int test_command(char *input_string);

    gets(input_string);

    test_command(input_string);

    printf("\n#sh ");
}

typedef uint8_t uint8_t;

int main()
{
    stdio_init_all();

    gpio_set_function(CONFIG_PWM_PIN, GPIO_FUNC_PWM);
    int slice = pwm_gpio_to_slice_num(CONFIG_PWM_PIN);
    int channel = pwm_gpio_to_channel(CONFIG_PWM_PIN);

    pwm_set_wrap(slice, PWM_COUNTER_MAX);                    // PWM 计数器溢出周期
    pwm_set_chan_level(slice, channel, PWM_COUNTER_MAX / 2); // PWM 占空比 (音量) = x / 计数器溢出周期
    pwm_set_enabled(slice, true);

    printf("\n#sh ");

    playScore();

    while (1)
    {
        test_command_handler();
    }
}

void beep(int freq, int delay)
{
    int slice = pwm_gpio_to_slice_num(CONFIG_PWM_PIN);
    int channel = pwm_gpio_to_channel(CONFIG_PWM_PIN);
    /**
     * 设置 PWM 时钟分频系数 (用来改变输出频率)
     *
     * 输入时钟 / 计数器溢出周期 = 最大输出频率
     * 最大输出频率 / 需要的频率 = 分频系数
     */
    if (freq <= 0)
    {
        pwm_set_chan_level(slice, channel, 0);
        pwm_set_clkdiv(slice, 0xffff);
    }
    else
    {
        pwm_set_chan_level(slice, channel, PWM_COUNTER_MAX / 2);
        pwm_set_clkdiv(slice, PWM_CLOCK / PWM_COUNTER_MAX / freq);
    }

    sleep_ms(delay);
}

const uint8_t score[] = {0x90, 64, 1, 96,  0x90, 66, 1, 96,  0x90, 67, 4, 34,  0x90, 66, 1, 96,  0x90, 67,  2, 193,
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

#define TUNE_OP_PLAYNOTE 0x90 /* play a note */
#define TUNE_OP_STOPNOTE 0x80 /* stop a note */
#define TUNE_OP_RESTART 0xe0  /* restart the score from the beginning */
#define TUNE_OP_STOP 0xf0     /* stop playing */

/**
 * @brief
 *
 * Table of midi note frequencies
 *
 * Ref: http://www.phy.mtu.edu/~suits/notefreqs.html
 */
const unsigned int midi_note2freq_table[128] = {
    16,    17,    18,    19,    21,    22,    23,    24,    26,    28,    29,    31,    33,    35,    37,    39,
    41,    44,    46,    49,    52,    55,    58,    62,    65,    69,    73,    78,    82,    87,    92,    98,
    104,   110,   117,   123,   131,   139,   147,   156,   165,   175,   185,   196,   208,   220,   233,   247,
    262,   277,   294,   311,   330,   349,   370,   392,   415,   440,   466,   494,   523,   554,   587,   622,
    659,   698,   740,   784,   831,   880,   932,   988,   1047,  1109,  1175,  1245,  1319,  1397,  1480,  1568,
    1661,  1760,  1865,  1976,  2093,  2217,  2349,  2489,  2637,  2794,  2960,  3136,  3322,  3520,  3729,  3951,
    4186,  4435,  4699,  4978,  5274,  5588,  5920,  6272,  6645,  7040,  7459,  7902,  8372,  8870,  9397,  9956,
    10548, 11175, 11840, 12544, 13290, 14080, 14917, 15804, 16744, 17740, 18795, 19912, 21096, 22351, 23680, 25088};

void playNote(uint8_t ch, uint8_t note, unsigned duration)
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

    int freq = midi_note2freq_table[note];

    beep(freq, duration);
}

void stopNote(uint8_t ch)
{
    beep(ch, 0);
}

void playScore()
{
    int index = 0;
    int arraySize = sizeof(score) / sizeof(score[0]);

    while (index < arraySize)
    {
        uint8_t dat = score[index++];
        uint8_t opcode = dat & 0xf0;
        uint8_t chan = dat & 0x0f;

        if (opcode == TUNE_OP_STOPNOTE)
        {
            stopNote(chan);
        }

        else if (opcode == TUNE_OP_PLAYNOTE)
        {
            uint8_t tone = score[index++];
            unsigned duration = ((unsigned)score[index] << 8) | score[index + 1];
            index += 2;
            playNote(chan, tone, duration);
        }
    }
}

/* __TEST_COMMAND__START__ */
#include "test_command.h"
static int play(int argc, char *argv[])
{
    playScore();
    return 0;
}
EXPORT_TEST_COMMAND(play, "play", "播放");
/* __TEST_COMMAND__END__ */
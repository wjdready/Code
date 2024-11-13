#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define SAMPLE_COUNT 480
#define PI 3.14159265358979323846

int main() {
    int16_t sine_wave[SAMPLE_COUNT];
    double amplitude = INT16_MAX; // 最大 int16_t 值
    double frequency = 1600;    // 频率
    double sample_rate = 48000; // 采样率
    double two_pi_f = 2.0 * PI * frequency / sample_rate;

    for (int i = 0; i < SAMPLE_COUNT; i++) {
        sine_wave[i] = (int16_t)(amplitude * sin(two_pi_f * i));
    }

    // 打印生成的正弦波数据
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        printf("%d ,", sine_wave[i]);
    }

    return 0;
}

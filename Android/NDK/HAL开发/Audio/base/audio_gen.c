#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define SAMPLE_COUNT 960
#define PI 3.14159265358979323846

int main() {
    int32_t sine_wave[SAMPLE_COUNT];
    double amplitude = INT32_MAX * 0.005; // 最大 int16_t 值
    double frequency = 1000;    // 频率
    double sample_rate = 48000; // 采样率
    double two_pi_f = 2.0 * PI * frequency / sample_rate;

    for (int i = 0; i < SAMPLE_COUNT; i += 4) {
        // sine_wave[i] = (int32_t)(amplitude * sin(two_pi_f * i));
        sine_wave[i + 0] = 0x00112233;
        sine_wave[i + 1] = 0x44556677;
        sine_wave[i + 2] = 0x8899AABB;
        sine_wave[i + 3] = 0xCCDDEEFF;
    }

    // 打印生成的正弦波数据
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        printf("0x%08x, ", sine_wave[i]);
    }
    

    return 0;
}


/* 


33 22 11 00 77 66 55 44 BB AA 99 88 FF EE DD CC 33 22 11 00 77 66 55 44 BB AA 99 88 FF EE DD CC 33 22 11 00 77 66 55 44 BB AA 99 88 FF EE DD CC 

ddeeff88 112233cc 55667700 99aabb44 ddeeff88 112233cc 55667700 99aabb44 ddeeff88 
112233cc 55667700 99aabb44 ddeeff88 112233cc 55667700 99aabb44 ddeeff88 112233cc 
55667700 99aabb44 ddeeff88 112233cc 55667700 99aabb44 ddeeff88 112233cc 55667700 
99aabb44 ddeeff88 112233cc 55667700 99aabb44 ddeeff88 112233cc 55667700 99aabb44 
ddeeff88 112233cc 55667700 99aabb44 ddeeff88 112233cc 55667700 99aabb44 ddeeff88 
112233cc 55667700 99aabb44 ddeeff88 112233cc 55667700 99aabb44 ddeeff88 112233cc 
55667700 99aabb44 ddeeff88 112233cc 55667700 99aabb44 ddeeff88 112233cc 55667700 
99aabb44 

*/
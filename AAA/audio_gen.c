#include <stdint.h>
#include <stdio.h>
#include <math.h> 

typedef enum {
    LSE_16,
    LSE_32
} audio_format_t;

// 【注意】 BUF 似乎不能太大，太大就不连续了, 48000, 长度 480 * 4 为宜
void audio_gen(int fs, int freq, audio_format_t format, int channel, void *buffer, int len)
{
    if (buffer == NULL || len <= 0 || fs <= 0 || freq <= 0 || channel <= 0) {
        return; // 处理无效输入
    }

    float step = (float)freq / fs;
    float x = 0;
    int count = format == LSE_16 ? len / (2 * channel) : len / (4 * channel);

    // int16_t *buffer_16 = (int8_t *)buffer;  // 修改为有符号 16 位整数指针
    // int32_t *buffer_32 = (int32_t *)buffer;  // 修改为有符号 32 位整数指针

    uint8_t *pbuf = (uint8_t *)buffer;

    for (int i = 0; i < count; i++) {
        float sample = sin(2 * M_PI * x);
        x += step;

        if (format == LSE_16) {
            int16_t value = (int16_t)(sample * (INT16_MAX >> 4)); // 转换为有符号 16 位整数
            for (int ch = 0; ch < channel; ch++) {
                // *buffer_16++ = value;
                pbuf[ch * 2 + i * 2] = value & 0xFF;
                pbuf[ch * 2 + i * 2 + 1] = (value >> 8) & 0xFF;
            }
        } else {
            int32_t value = (int32_t)(sample * (INT32_MAX >> 4)); // 转换为有符号 32 位整数
            for (int ch = 0; ch < channel; ch++) {
                // *buffer_32++ = value;
                pbuf[ch * 4 + i * 4] = value & 0xFF;
                pbuf[ch * 4 + i * 4 + 1] = (value >> 8) & 0xFF;
                pbuf[ch * 4 + i * 4 + 2] = (value >> 16) & 0xFF;
                pbuf[ch * 4 + i * 4 + 3] = (value >> 24) & 0xFF;
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    // 以二进制写入模式打开文件
    FILE *fp = fopen("audio.pcm", "wb");
    if (fp == NULL) {
        perror("无法打开文件");
        return 1;
    }

    // 定义缓冲区 - 使用与LSE_32匹配的int32_t类型
    int16_t buf[48000];  // 32位缓冲区，保持总字节数不变

    // 生成音频数据
    audio_gen(48000, 1000, LSE_16, 1, buf, sizeof(buf));

    // 打印生成的数据（调试用）
    // for (int i = 0; i < (sizeof(buf) / 4); i++) {
    //     printf("%ld ,", (long)buf[i]);
    // }

    // 将缓冲区数据写入文件
    size_t bytes_written = fwrite(buf, sizeof(unsigned char), sizeof(buf), fp);
    if (bytes_written < sizeof(buf)) {
        perror("写入文件时出错");
        fclose(fp);
        return 1;
    }

    // 关闭文件
    fclose(fp);
    printf("数据已成功以二进制模式写入 audio.pcm\n");

    return 0;
}
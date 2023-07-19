#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>

/* wav 文件头默认配置 */
#define SUBCHUNK1_SIZE         (16)
#define AUDIO_FORMAT           (1)      /* PCM */
#define SUBCHUNK2_SIZE         (0)
#define SUBCHUNK_FACT_SIZE     (460)
#define CHUNK_SIZE             (4 + (8 + SUBCHUNK1_SIZE) + (8 + SUBCHUNK_FACT_SIZE) + (8 + SUBCHUNK2_SIZE))
#define SAMPLE_RATE            (48000)
#define BITS_PER_SAMPLE        (32)
#define NUM_CHANNELS           (1)

#define BYTE_RATE              (SAMPLE_RATE * NUM_CHANNELS * BITS_PER_SAMPLE / 8)
#define BLOCK_ALIGN            (NUM_CHANNELS * BITS_PER_SAMPLE / 8)

typedef struct 
{
    // RIFF块 (RIFF-Chunk)
    char  chunk_id[4];          // 4 字节, RIFF 标志
    int   chunk_size;           // 4 字节, 总chunk大小
    char  format[4];            // 4 字节, WAVE 标志

    // 格式化块 (Format-Chunk) 16 字节
    char  subchunk1_id[4];      // 4 字节, fmt 标志
    int   subchunk1_size;       // 4 字节, subchunk 大小
    short audio_format;         // 2 字节, 音频格式类别 
    short num_channels;         // 2 字节, 声道数
    int   sample_rate;          // 4 字节, 采样率
    int   byte_rate;            // 4 字节, 位速, 传输速率 
    short block_align;          // 2 字节, 一个采样多声道数据块大小, 数据块对齐 
    short bits_per_sample;      // 2 字节, 一个采样占的 bit 数

    // 附加块(Fact-Chunk) 0 字节
    char  subchunk_fact_id[4];  // 4 字节, data 标志
    int   subchunk_fact_size;   // subchunk 大小
    char  fact_fill[SUBCHUNK_FACT_SIZE];

    // 数据块(Data-Chunk) 
    char  subchunk2_id[4];       // 4 字节, data 标志
    int   subchunk2_size;        // subchunk 大小

} wavfile_header_t;


/**
 * \brief 正弦信号数据采集, freq 信号频率, fs 采样频率, vpp 信号幅度
 */
void sample_sin(float *dat, int len, float freq, float fs, float vpp)
{
    static uint32_t i = 0;
 
    const float pi = 3.14159265358979323846f;
 
    int n = fs / freq;
 
    for (uint32_t j = 0; j < len; i++, j++)
    {
        float x = i * 2.0f * pi / n;
        dat[j] = vpp * cosf(x);
        if(i >= n )
        {
            i = 0;
        }
    }
}


static const wavfile_header_t wav_header_default = {
    .chunk_id         = "RIFF",
    .chunk_size       = CHUNK_SIZE,
    .format           = "WAVE",

    .subchunk1_id     = "fmt ",
    .subchunk1_size   = SUBCHUNK1_SIZE,
    .audio_format     = AUDIO_FORMAT,
    .num_channels     = NUM_CHANNELS,
    .sample_rate      = SAMPLE_RATE,
    .byte_rate        = BYTE_RATE,
    .block_align      = BLOCK_ALIGN,
    .bits_per_sample  = BITS_PER_SAMPLE,

    .subchunk_fact_id = "fact",
    .subchunk_fact_size = SUBCHUNK_FACT_SIZE,

    .subchunk2_id     = "data",
    .subchunk2_size   = SUBCHUNK2_SIZE,
};

enum {
    RECORD_AUDIO_FORMAT_48KHz_32BIT,
    RECORD_AUDIO_FORMAT_24KHz_16BIT,
    RECORD_AUDIO_FORMAT_12KHz_16BIT,
};
typedef int record_audio_format_t;

#define BUF_LEN 480
float buf[BUF_LEN];

typedef struct 
{
    wavfile_header_t header;
    FILE *fp;
    record_audio_format_t format;   /* 格式 */
    float gain;                     /* 增益 */ 
    uint32_t frame_count;           /* 累计写入的帧数据量 */
    uint8_t buf[BUF_LEN * 4];       /* 最大缓存 */
}record_audio_t;

record_audio_t audio;

void record_audio_write(float *dat, int len)
{
    int buf_len;

    if(audio.format == RECORD_AUDIO_FORMAT_48KHz_32BIT)
    {
        buf_len = 4 * len;

        int32_t *buf = (int32_t *)audio.buf;
        
        for (int i = 0; i < len; i += 1)
        {
            float d = dat[i] * audio.gain;
            buf[i] = d > INT32_MAX ? INT32_MAX : (d < -INT32_MAX ? -INT32_MAX : d);
        }
    }

    else if(audio.format == RECORD_AUDIO_FORMAT_24KHz_16BIT)
    {
        buf_len = 2 * len / 2;

        int16_t *buf = (int16_t *)audio.buf;

        /* 24KHz 等间隔跳过 */
        for (int i = 0, j = 0; i < len; i += 2, j++)
        {
            float d = dat[i] * ((float)INT16_MAX / (float)INT32_MAX) * audio.gain;
            buf[j] = d > INT16_MAX ? INT16_MAX : (d < -INT16_MAX ? -INT16_MAX : d);
        }
    }

    /* 12K, 16bit */
    else
    {
        buf_len = 2 * len / 4;

        int16_t *buf = (int16_t *)audio.buf;

        for (int i = 0, j = 0; i < len; i += 4, j++)
        {
            float d = dat[i] * ((float)INT16_MAX / (float)INT32_MAX) * audio.gain;
            buf[j] = d > INT16_MAX ? INT16_MAX : (d < -INT16_MAX ? -INT16_MAX : d);
        }
    }
    
    fwrite(audio.buf, buf_len, 1, audio.fp);
    audio.frame_count += len;
}


void record_audio_stop(void)
{
    wavfile_header_t *h = &audio.header;

    if(audio.format == RECORD_AUDIO_FORMAT_24KHz_16BIT)
    {
        h->sample_rate = 24000;
        h->bits_per_sample = 16;
    }
    else if(audio.format == RECORD_AUDIO_FORMAT_12KHz_16BIT)
    {
        h->sample_rate = 12000;
        h->bits_per_sample = 16;
    }

    /* 更新头信息 */
    h->subchunk2_size = audio.frame_count * h->num_channels * h->bits_per_sample / 8;
    h->byte_rate = (h->sample_rate * h->num_channels * h->bits_per_sample / 8);
    h->block_align = (h->num_channels * h->bits_per_sample / 8);

    /* 最后重新计算总大小 */
    h->chunk_size = (4 + (8 + h->subchunk1_size) + (8 + h->subchunk_fact_size) + (8 + h->subchunk2_size));
    
    /* 更新头信息 */
    fseek(audio.fp, 0, SEEK_SET);
    int n = fwrite(&audio.header,  sizeof(wavfile_header_t), 1, audio.fp);
    fclose(audio.fp);
}

int main(int argc, char const *argv[])
{
    audio.fp = fopen("./test.wav", "wb");
    audio.header = wav_header_default;

    /* 先写入固定大小的文件头占位置 */
    fwrite(&audio.header,  sizeof(wavfile_header_t), 1, audio.fp);
    
    /* 配置信息 */
    audio.format = RECORD_AUDIO_FORMAT_12KHz_16BIT;
    audio.frame_count = 0;

    int dB = 32;        /* 增益 dB */ 
    audio.gain = 1.0f * powf(10, dB / 20.0f);
    printf("gain %d db,  %f bei\n", dB,  audio.gain);

    /* 写入数据 */
    for (int i = 0; i < 1000; i++)
    {   
        /* 采样数据 */
        sample_sin(buf, BUF_LEN, 1000, SAMPLE_RATE, 10000000);

        /* 写入数据 */
        record_audio_write(buf, BUF_LEN);
    }

    record_audio_stop();
    
    return 0;
}

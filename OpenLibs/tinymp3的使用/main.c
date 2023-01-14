#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>
#include "shine_mp3.h"

/**
 * \brief 正弦信号数据采集, freq 信号频率, fs 采样频率, vpp 信号幅度
 */
void sample_sin(int16_t *dat, int len, float freq, float fs, float vpp)
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

/* Print some info about what we're going to encode */
static void check_config(shine_config_t *config) 
{
    static char *version_names[4] = {"2.5", "reserved", "II", "I"};
    static char *mode_names[4] = {"stereo", "joint-stereo", "dual-channel", "mono"};
    static char *demp_names[4] = {"none", "50/15us", "", "CITT"};

    printf("MPEG-%s layer III, %s  Psychoacoustic Model: Shine\n",
           version_names[shine_check_config(config->wave.samplerate, config->mpeg.bitr)],
           mode_names[config->mpeg.mode]);
    printf("Bitrate: %d kbps  ", config->mpeg.bitr);
    printf("De-emphasis: %s   %s %s\n",
           demp_names[config->mpeg.emph],
           ((config->mpeg.original) ? "Original" : ""),
           ((config->mpeg.copyright) ? "(C)" : ""));
    // printf("Encoding \"%s\" to \"%s\"\n", infname, outfname);
}


#define SAMPLE_RATE 48000
#define CHANNELS 1
#define BUF_LEN 48000

int main(int argc, char const *argv[])
{
    shine_config_t config;
    shine_t s;
    int16_t buf[BUF_LEN];

    shine_set_config_mpeg_defaults(&config.mpeg);
    config.mpeg.mode = MONO;
    config.wave.samplerate = SAMPLE_RATE;
    config.wave.channels = CHANNELS;
    
    /* See if samplerate and bitrate are valid */
    if (shine_check_config(config.wave.samplerate, config.mpeg.bitr) < 0)
        printf("Unsupported samplerate/bitrate configuration.\n");

    s = shine_initialise(&config);

    check_config(&config);

    int samples_per_pass = shine_samples_per_pass(s) * CHANNELS;

    printf("samples_per_pass: %d\n", samples_per_pass);
    
    FILE *fp = fopen("test.mp3", "w");

    if(fp == NULL)
        return -1;

    int written;
    uint8_t *data;

    /* 写入数据 */
    for (int i = 0; i < 100*4; i++)
    {   
        /* 采样数据 */
        sample_sin(buf, samples_per_pass, 1000, SAMPLE_RATE, 10000);

        data = shine_encode_buffer_interleaved(s, buf, &written);
        printf("written %d\n", written);
        if ( fwrite(data, 1, written, fp) != written )
        {
            fprintf(stderr, "shineenc: write error\n");
            return 1;
        }
    }

    /* Flush and write remaining data. */
    data = shine_flush(s, &written);
    fwrite(data, 1, written, fp);
    printf("written %d\n", written);

    /* Close encoder. */
    shine_close(s);

    /* Close the MP3 file */
    fclose(fp);

    return 0;
}

/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_NDEBUG 0
#define LOG_TAG "audio_hw"

#include <errno.h>
#include <malloc.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>
#include <cutils/log.h>
#include <cutils/str_parms.h>
#include <hardware/hardware.h>
#include <system/audio.h>
#include <hardware/audio.h>

struct stub_audio_device {
    struct audio_hw_device device;
};

struct stub_stream_out {
    struct audio_stream_out stream;
    int64_t last_write_time_us;
};

struct stub_stream_in {
    struct audio_stream_in stream;
    int64_t last_read_time_us;
};

static uint32_t out_get_sample_rate(const struct audio_stream *stream)
{
    return 48000;
}

static int out_set_sample_rate(struct audio_stream *stream, uint32_t rate)
{
    // ALOGD("out_set_sample_rate: %d", 0);
    return 0;
}

static size_t out_get_buffer_size(const struct audio_stream *stream)
{
    // ALOGD("out_get_buffer_size: %d", 4096);
    return 4096;
}

static audio_channel_mask_t out_get_channels(const struct audio_stream *stream)
{
    // ALOGD("out_get_channels AUDIO_CHANNEL_OUT_STEREO");
    return AUDIO_CHANNEL_OUT_STEREO;
}

static audio_format_t out_get_format(const struct audio_stream *stream)
{
    // ALOGD("out_get_format AUDIO_FORMAT_PCM_16_BIT = %d", AUDIO_FORMAT_PCM_16_BIT);
    return AUDIO_FORMAT_PCM_16_BIT;
}

static int out_set_format(struct audio_stream *stream, audio_format_t format)
{
    // ALOGD("out_set_format: %d",format);
    return 0;
}

static int out_standby(struct audio_stream *stream)
{
    // ALOGD("out_standby");
    // out->last_write_time_us = 0; unnecessary as a stale write time has same effect
    return 0;
}

static int out_dump(const struct audio_stream *stream, int fd)
{
    // ALOGD("out_dump");
    return 0;
}

static int out_set_parameters(struct audio_stream *stream, const char *kvpairs)
{
    // ALOGD("out_set_parameters");
    return 0;
}

static char * out_get_parameters(const struct audio_stream *stream, const char *keys)
{
    // ALOGD("out_get_parameters");
    return strdup("");
}

static uint32_t out_get_latency(const struct audio_stream_out *stream)
{
    // ALOGD("out_get_latency");
    return 0;
}

static int out_set_volume(struct audio_stream_out *stream, float left,
                          float right)
{
    // ALOGD("out_set_volume: Left:%f Right:%f", left, right);
    return 0;
}

static ssize_t out_write(struct audio_stream_out *stream, const void* buffer,
                         size_t bytes)
{
    // ALOGD("out_write: bytes: %zu", bytes);

    /* XXX: fake timing for audio output */
    struct stub_stream_out *out = (struct stub_stream_out *)stream;
    struct timespec t = { .tv_sec = 0, .tv_nsec = 0 };
    clock_gettime(CLOCK_MONOTONIC, &t);
    const int64_t now = (t.tv_sec * 1000000000LL + t.tv_nsec) / 1000;
    const int64_t elapsed_time_since_last_write = now - out->last_write_time_us;
    int64_t sleep_time = bytes * 1000000LL / audio_stream_out_frame_size(stream) /
               out_get_sample_rate(&stream->common) - elapsed_time_since_last_write;
    if (sleep_time > 0) {
        usleep(sleep_time);
    } else {
        // we don't sleep when we exit standby (this is typical for a real alsa buffer).
        sleep_time = 0;
    }
    out->last_write_time_us = now + sleep_time;
    // last_write_time_us is an approximation of when the (simulated) alsa
    // buffer is believed completely full. The usleep above waits for more space
    // in the buffer, but by the end of the sleep the buffer is considered
    // topped-off.
    //
    // On the subsequent out_write(), we measure the elapsed time spent in
    // the mixer. This is subtracted from the sleep estimate based on frames,
    // thereby accounting for drain in the alsa buffer during mixing.
    // This is a crude approximation; we don't handle underruns precisely.
    return bytes;
}

static int out_get_render_position(const struct audio_stream_out *stream,
                                   uint32_t *dsp_frames)
{
    *dsp_frames = 0;
    // ALOGD("out_get_render_position: dsp_frames: %p", dsp_frames);
    return -EINVAL;
}

static int out_add_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    // ALOGD("out_add_audio_effect: %p", effect);
    return 0;
}

static int out_remove_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    // ALOGD("out_remove_audio_effect: %p", effect);
    return 0;
}

static int out_get_next_write_timestamp(const struct audio_stream_out *stream,
                                        int64_t *timestamp)
{
    *timestamp = 0;
    // ALOGD("out_get_next_write_timestamp: %ld", (long int)(*timestamp));
    return -EINVAL;
}

/** audio_stream_in implementation **/
static uint32_t in_get_sample_rate(const struct audio_stream *stream)
{
    ALOGD("in_get_sample_rate %d", 48000);
    return 48000;
}

static int in_set_sample_rate(struct audio_stream *stream, uint32_t rate)
{
    ALOGD("in_set_sample_rate: %d", rate);
    return 0;
}

static size_t in_get_buffer_size(const struct audio_stream *stream)
{
    int fs_count = 480;
    int bytes_per_sample = 4;
    int channel_count = 4;
    int buffer_size = fs_count * bytes_per_sample * channel_count;
    ALOGD("in_get_buffer_size: %d", buffer_size);
    return buffer_size;
}

static audio_channel_mask_t in_get_channels(const struct audio_stream *stream)
{
    uint32_t chmask = AUDIO_CHANNEL_IN_LEFT | AUDIO_CHANNEL_IN_RIGHT | AUDIO_CHANNEL_IN_FRONT | AUDIO_CHANNEL_IN_BACK;
    ALOGD("in_get_channels: %d", chmask);
    return chmask;
}

static audio_format_t in_get_format(const struct audio_stream *stream)
{
    ALOGD("in_get_format AUDIO_FORMAT_PCM_32_BIT = %d stream %p", AUDIO_FORMAT_PCM_32_BIT, stream);
    return AUDIO_FORMAT_PCM_32_BIT;
}

/* 似乎没用到 */
static int in_set_format(struct audio_stream *stream, audio_format_t format)
{
    ALOGD("in_set_format %d", format);
    return 0;
}

static int in_standby(struct audio_stream *stream)
{
    struct stub_stream_in *in = (struct stub_stream_in *)stream;
    in->last_read_time_us = 0;
    ALOGD("in_standby");
    return 0;
}

/* 似乎没用到 */
static int in_dump(const struct audio_stream *stream, int fd)
{
    ALOGD("in_dump");
    return 0;
}

static int in_set_parameters(struct audio_stream *stream, const char *kvpairs)
{
    struct str_parms *parms;
    char value[32];
    unsigned int val;
    int ret;

    ALOGD("======= %s: kvpairs = %s", __func__, kvpairs);

    parms = str_parms_create_str(kvpairs);

    //set channel_mask
    ret = str_parms_get_str(parms, AUDIO_PARAMETER_STREAM_CHANNELS, 
                            value, sizeof(value));
    if (ret >= 0) {
        val = atoi(value);
        ALOGD("======= set param channel_mask %d", val);
    }

    // set sample rate
    ret = str_parms_get_str(parms, AUDIO_PARAMETER_STREAM_SAMPLING_RATE,
                            value, sizeof(value));
    if (ret >= 0) {
        val = atoi(value);
        ALOGD("======= set param sample rate %d", val);
    }

    // 输入源
    ret = str_parms_get_str(parms, AUDIO_PARAMETER_STREAM_INPUT_SOURCE,
                        value, sizeof(value));
    if (ret >= 0) {
        val = atoi(value);
        ALOGD("======= set input source %d", val);
    }

    ret = str_parms_get_str(parms, AUDIO_PARAMETER_STREAM_ROUTING,
                            value, sizeof(value));
    if (ret >= 0) {
        val = atoi(value);
        ALOGD("======= set stream routing %d", val);
    }

    return 0;
}

static char * in_get_parameters(const struct audio_stream *stream,
                                const char *keys)
{
    ALOGD("in_get_parameters %s", keys);
    return strdup("");
}

static int in_set_gain(struct audio_stream_in *stream, float gain)
{
    ALOGD("in_set_gain");
    return 0;
}
// 三角波
static int32_t audio_data_sin0[] = {
0 ,50 ,100 ,150 ,200 ,250 ,300 ,350 ,400 ,450 ,500 ,550 ,600 ,650 ,700 ,750 ,800 ,850 ,900 ,950 ,1000 ,1050 ,1100 ,1150 ,1200 ,1250 ,1300 ,1350 ,1400 ,1450 ,1500 ,1550 ,1600 ,1650 ,1700 ,1750 ,1800 ,1850 ,1900 ,1950 ,2000 ,2050 ,2100 ,2150 ,2200 ,2250 ,2300 ,2350 ,2400 ,2450 ,2500 ,2550 ,2600 ,2650 ,2700 ,2750 ,2800 ,2850 ,2900 ,2950 ,3000 ,3050 ,3100 ,3150 ,3200 ,3250 ,3300 ,3350 ,3400 ,3450 ,3500 ,3550 ,3600 ,3650 ,3700 ,3750 ,3800 ,3850 ,3900 ,3950 ,4000 ,4050 ,4100 ,4150 ,4200 ,4250 ,4300 ,4350 ,4400 ,4450 ,4500 ,4550 ,4600 ,4650 ,4700 ,4750 ,4800 ,4850 ,4900 ,4950 ,5000 ,5050 ,5100 ,5150 ,5200 ,5250 ,5300 ,5350 ,5400 ,5450 ,5500 ,5550 ,5600 ,5650 ,5700 ,5750 ,5800 ,5850 ,5900 ,5950 ,6000 ,6050 ,6100 ,6150 ,6200 ,6250 ,6300 ,6350 ,6400 ,6450 ,6500 ,6550 ,6600 ,6650 ,6700 ,6750 ,6800 ,6850 ,6900 ,6950 ,7000 ,7050 ,7100 ,7150 ,7200 ,7250 ,7300 ,7350 ,7400 ,7450 ,7500 ,7550 ,7600 ,7650 ,7700 ,7750 ,7800 ,7850 ,7900 ,7950 ,8000 ,8050 ,8100 ,8150 ,8200 ,8250 ,8300 ,8350 ,8400 ,8450 ,8500 ,8550 ,8600 ,8650 ,8700 ,8750 ,8800 ,8850 ,8900 ,8950 ,9000 ,9050 ,9100 ,9150 ,9200 ,9250 ,9300 ,9350 ,9400 ,9450 ,9500 ,9550 ,9600 ,9650 ,9700 ,9750 ,9800 ,9850 ,9900 ,9950 ,10000 ,10050 ,10100 ,10150 ,10200 ,10250 ,10300 ,10350 ,10400 ,10450 ,10500 ,10550 ,10600 ,10650 ,10700 ,10750 ,10800 ,10850 ,10900 ,10950 ,11000 ,11050 ,11100 ,11150 ,11200 ,11250 ,11300 ,11350 ,11400 ,11450 ,11500 ,11550 ,11600 ,11650 ,11700 ,11750 ,11800 ,11850 ,11900 ,11950 ,12000 ,12050 ,12100 ,12150 ,12200 ,12250 ,12300 ,12350 ,12400 ,12450 ,12500 ,12550 ,12600 ,12650 ,12700 ,12750 ,12800 ,12850 ,12900 ,12950 ,13000 ,13050 ,13100 ,13150 ,13200 ,13250 ,13300 ,13350 ,13400 ,13450 ,13500 ,13550 ,13600 ,13650 ,13700 ,13750 ,13800 ,13850 ,13900 ,13950 ,14000 ,14050 ,14100 ,14150 ,14200 ,14250 ,14300 ,14350 ,14400 ,14450 ,14500 ,14550 ,14600 ,14650 ,14700 ,14750 ,14800 ,14850 ,14900 ,14950 ,15000 ,15050 ,15100 ,15150 ,15200 ,15250 ,15300 ,15350 ,15400 ,15450 ,15500 ,15550 ,15600 ,15650 ,15700 ,15750 ,15800 ,15850 ,15900 ,15950 ,16000 ,16050 ,16100 ,16150 ,16200 ,16250 ,16300 ,16350 ,16400 ,16450 ,16500 ,16550 ,16600 ,16650 ,16700 ,16750 ,16800 ,16850 ,16900 ,16950 ,17000 ,17050 ,17100 ,17150 ,17200 ,17250 ,17300 ,17350 ,17400 ,17450 ,17500 ,17550 ,17600 ,17650 ,17700 ,17750 ,17800 ,17850 ,17900 ,17950 ,18000 ,18050 ,18100 ,18150 ,18200 ,18250 ,18300 ,18350 ,18400 ,18450 ,18500 ,18550 ,18600 ,18650 ,18700 ,18750 ,18800 ,18850 ,18900 ,18950 ,19000 ,19050 ,19100 ,19150 ,19200 ,19250 ,19300 ,19350 ,19400 ,19450 ,19500 ,19550 ,19600 ,19650 ,19700 ,19750 ,19800 ,19850 ,19900 ,19950 ,20000 ,20050 ,20100 ,20150 ,20200 ,20250 ,20300 ,20350 ,20400 ,20450 ,20500 ,20550 ,20600 ,20650 ,20700 ,20750 ,20800 ,20850 ,20900 ,20950 ,21000 ,21050 ,21100 ,21150 ,21200 ,21250 ,21300 ,21350 ,21400 ,21450 ,21500 ,21550 ,21600 ,21650 ,21700 ,21750 ,21800 ,21850 ,21900 ,21950 ,22000 ,22050 ,22100 ,22150 ,22200 ,22250 ,22300 ,22350 ,22400 ,22450 ,22500 ,22550 ,22600 ,22650 ,22700 ,22750 ,22800 ,22850 ,22900 ,22950 ,23000 ,23050 ,23100 ,23150 ,23200 ,23250 ,23300 ,23350 ,23400 ,23450 ,23500 ,23550 ,23600 ,23650 ,23700 ,23750 ,23800 ,23850 ,23900 ,23950 ,
};

// 正弦波
static int32_t audio_data_sin1[] = {
0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,0 ,446486956 ,873460289 ,1262259217 ,1595891360 ,1859775392 ,2042378316 ,2135719506 ,2135719506 ,2042378316 ,1859775392 ,1595891360 ,1262259217 ,873460289 ,446486956 ,0 ,-446486956 ,-873460289 ,-1262259217 ,-1595891360 ,-1859775392 ,-2042378316 ,-2135719506 ,-2135719506 ,-2042378316 ,-1859775392 ,-1595891360 ,-1262259217 ,-873460289 ,-446486956 ,
};

// 正弦波 4K
static int32_t audio_data_sin2[] = {
0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 ,0 ,1073741823 ,1859775392 ,2147483647 ,1859775392 ,1073741823 ,0 ,-1073741823 ,-1859775392 ,-2147483647 ,-1859775392 ,-1073741823 
};

// 方波
static int32_t audio_data_sin3[] = {
0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,32587 ,
};

static int32_t audio_data_buffer[480*4];

int32_t signal_count = 0;

// 需要根据数据量进行吞吐
// 一般来说，上层总是每次接收一半，以完整接收
// in_read 应该是被一个线程频繁调用的
// 不能读的太快，读的太快就需要延迟
// audio_stream_in_frame_size 音频帧大小 = 通道数 * 每通道多少字节
static ssize_t in_read(struct audio_stream_in *stream, void* buffer,
                       size_t bytes)
{
    ALOGD("in_read: bytes %zu, fram_size %d", bytes, audio_stream_in_frame_size(stream));

    /* XXX: fake timing for audio input */
    struct stub_stream_in *in = (struct stub_stream_in *)stream;
    struct timespec t = { .tv_sec = 0, .tv_nsec = 0 };
    clock_gettime(CLOCK_MONOTONIC, &t);
    const int64_t now = (t.tv_sec * 1000000000LL + t.tv_nsec) / 1000;

    // we do a full sleep when exiting standby.
    const bool standby = in->last_read_time_us == 0;
    const int64_t elapsed_time_since_last_read = standby ?
            0 : now - in->last_read_time_us;
    // sleep 时间 = 这次接收的这么多 bytes 所占的音频时间 - 距离上层读取所经过的时间
    int64_t sleep_time = bytes * 1000000LL / audio_stream_in_frame_size(stream) /
            in_get_sample_rate(&stream->common) - elapsed_time_since_last_read;
    if (sleep_time > 0) {
        usleep(sleep_time);
    } else {
        sleep_time = 0;
    }
    in->last_read_time_us = now + sleep_time;
    // last_read_time_us is an approximation of when the (simulated) alsa
    // buffer is drained by the read, and is empty.
    //
    // On the subsequent in_read(), we measure the elapsed time spent in
    // the recording thread. This is subtracted from the sleep estimate based on frames,
    // thereby accounting for fill in the alsa buffer during the interim.

    // memset(buffer, 0, bytes);
    for (int i = 0; i < 480; i++)
    {
        audio_data_buffer[4 * i + 0] = audio_data_sin0[i];
        audio_data_buffer[4 * i + 1] = audio_data_sin1[i];
        audio_data_buffer[4 * i + 2] = audio_data_sin2[i];
        audio_data_buffer[4 * i + 3] = audio_data_sin3[i];
        signal_count += 1000;
    }

    memcpy(buffer, audio_data_buffer, sizeof(audio_data_buffer));

    return bytes;
}

static uint32_t in_get_input_frames_lost(struct audio_stream_in *stream)
{
    ALOGD("in_get_input_frames_lost");
    return 0;
}

static int in_add_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    ALOGD("in_add_audio_effect");
    return 0;
}

static int in_remove_audio_effect(const struct audio_stream *stream, effect_handle_t effect)
{
    ALOGD("in_remove_audio_effect");
    return 0;
}

static int adev_open_output_stream(struct audio_hw_device *dev,
                                   audio_io_handle_t handle,
                                   audio_devices_t devices,
                                   audio_output_flags_t flags,
                                   struct audio_config *config,
                                   struct audio_stream_out **stream_out,
                                   const char *address __unused)
{
    ALOGD("adev_open_output_stream...");

    struct stub_audio_device *ladev = (struct stub_audio_device *)dev;
    struct stub_stream_out *out;
    int ret;

    out = (struct stub_stream_out *)calloc(1, sizeof(struct stub_stream_out));
    if (!out)
        return -ENOMEM;

    out->stream.common.get_sample_rate = out_get_sample_rate;
    out->stream.common.set_sample_rate = out_set_sample_rate;
    out->stream.common.get_buffer_size = out_get_buffer_size;
    out->stream.common.get_channels = out_get_channels;
    out->stream.common.get_format = out_get_format;
    out->stream.common.set_format = out_set_format;
    out->stream.common.standby = out_standby;
    out->stream.common.dump = out_dump;
    out->stream.common.set_parameters = out_set_parameters;
    out->stream.common.get_parameters = out_get_parameters;
    out->stream.common.add_audio_effect = out_add_audio_effect;
    out->stream.common.remove_audio_effect = out_remove_audio_effect;
    out->stream.get_latency = out_get_latency;
    out->stream.set_volume = out_set_volume;
    out->stream.write = out_write;
    out->stream.get_render_position = out_get_render_position;
    out->stream.get_next_write_timestamp = out_get_next_write_timestamp;

    *stream_out = &out->stream;
    return 0;

err_open:
    free(out);
    *stream_out = NULL;
    return ret;
}

static void adev_close_output_stream(struct audio_hw_device *dev,
                                     struct audio_stream_out *stream)
{
    ALOGD("adev_close_output_stream...");
    free(stream);
}

static int adev_set_parameters(struct audio_hw_device *dev, const char *kvpairs)
{
    ALOGD("======= %s: kvpairs = %s", __func__, kvpairs);
    return 0;
}

static char * adev_get_parameters(const struct audio_hw_device *dev,
                                  const char *keys)
{
    ALOGD("adev_get_parameters");
    return strdup("");
}

/* 检查是否初始化完成 */
static int adev_init_check(const struct audio_hw_device *dev)
{
    ALOGD("adev_init_check");
    return 0;
}

static int adev_set_voice_volume(struct audio_hw_device *dev, float volume)
{
    ALOGD("adev_set_voice_volume: %f", volume);
    return 0;
}

static int adev_set_master_volume(struct audio_hw_device *dev, float volume)
{
    ALOGD("adev_set_master_volume: %f", volume);
    return 0;
}

static int adev_get_master_volume(struct audio_hw_device *dev, float *volume)
{
    ALOGD("adev_get_master_volume: %f", *volume);
    return 0;
}

static int adev_set_master_mute(struct audio_hw_device *dev, bool muted)
{
    ALOGD("adev_set_master_mute: %d", muted);
    return 0;
}

static int adev_get_master_mute(struct audio_hw_device *dev, bool *muted)
{
    ALOGD("adev_get_master_mute: %d", *muted);
    return 0;
}

static int adev_set_mode(struct audio_hw_device *dev, audio_mode_t mode)
{
    ALOGD("adev_set_mode: %d", mode);
    return 0;
}

static int adev_set_mic_mute(struct audio_hw_device *dev, bool state)
{
    ALOGD("adev_set_mic_mute: %d",state);
    return 0;
}

static int adev_get_mic_mute(const struct audio_hw_device *dev, bool *state)
{
    ALOGD("adev_get_mic_mute");
    return 0;
}

static size_t adev_get_input_buffer_size(const struct audio_hw_device *dev,
                                         const struct audio_config *config)
{
    int fs_count = 480;
    int bytes_per_sample = 4;
    int channel_count = 4;
    int buffer_size = fs_count * bytes_per_sample * channel_count;
    ALOGD("adev_get_input_buffer_size: %d", buffer_size);
    return buffer_size;
}

static int adev_open_input_stream(struct audio_hw_device *dev,
                                  audio_io_handle_t handle,
                                  audio_devices_t devices,
                                  struct audio_config *config,
                                  struct audio_stream_in **stream_in,
                                  audio_input_flags_t flags __unused,
                                  const char *address __unused,
                                  audio_source_t source __unused)
{
    ALOGD("==== adev_open_input_stream...");

    ALOGD("==== config:");
    ALOGD("==== sample_rate %d :", config->sample_rate);
    ALOGD("==== channel_mask %d:", config->channel_mask);
    ALOGD("==== format %d      :", config->format);
    ALOGD("==== frame_count %d :", config->frame_count);

    struct stub_audio_device *ladev = (struct stub_audio_device *)dev;
    struct stub_stream_in *in;
    int ret;

    in = (struct stub_stream_in *)calloc(1, sizeof(struct stub_stream_in));
    if (!in)
        return -ENOMEM;

    in->stream.common.get_sample_rate = in_get_sample_rate;
    in->stream.common.set_sample_rate = in_set_sample_rate;
    in->stream.common.get_buffer_size = in_get_buffer_size;
    in->stream.common.get_channels = in_get_channels;
    in->stream.common.get_format = in_get_format;
    in->stream.common.set_format = in_set_format;
    in->stream.common.standby = in_standby;
    in->stream.common.dump = in_dump;
    in->stream.common.set_parameters = in_set_parameters;
    in->stream.common.get_parameters = in_get_parameters;
    in->stream.common.add_audio_effect = in_add_audio_effect;
    in->stream.common.remove_audio_effect = in_remove_audio_effect;
    in->stream.set_gain = in_set_gain;
    in->stream.read = in_read;
    in->stream.get_input_frames_lost = in_get_input_frames_lost;

    *stream_in = &in->stream;
    return 0;

err_open:
    free(in);
    *stream_in = NULL;
    return ret;
}

static void adev_close_input_stream(struct audio_hw_device *dev,
                                   struct audio_stream_in *in)
{
    ALOGD("adev_close_input_stream...");
    return;
}

static int adev_dump(const audio_hw_device_t *device, int fd)
{
    ALOGD("adev_dump");
    return 0;
}

static int adev_close(hw_device_t *device)
{
    ALOGD("adev_close");
    free(device);
    return 0;
}

static int adev_open(const hw_module_t* module, const char* name,
                     hw_device_t** device)
{
    ALOGD("adev_open: %s", name);

    ALOGD("======== adev_open 3: %s", name);

    struct stub_audio_device *adev;
    int ret;

    if (strcmp(name, AUDIO_HARDWARE_INTERFACE) != 0)
        return -EINVAL;

    adev = calloc(1, sizeof(struct stub_audio_device));
    if (!adev)
        return -ENOMEM;

    adev->device.common.tag = HARDWARE_DEVICE_TAG;
    adev->device.common.version = AUDIO_DEVICE_API_VERSION_2_0;
    adev->device.common.module = (struct hw_module_t *) module;
    adev->device.common.close = adev_close;

    adev->device.init_check = adev_init_check;
    adev->device.set_voice_volume = adev_set_voice_volume;
    adev->device.set_master_volume = adev_set_master_volume;
    adev->device.get_master_volume = adev_get_master_volume;
    adev->device.set_master_mute = adev_set_master_mute;
    adev->device.get_master_mute = adev_get_master_mute;
    adev->device.set_mode = adev_set_mode;
    adev->device.set_mic_mute = adev_set_mic_mute;
    adev->device.get_mic_mute = adev_get_mic_mute;
    adev->device.set_parameters = adev_set_parameters;
    adev->device.get_parameters = adev_get_parameters;
    adev->device.get_input_buffer_size = adev_get_input_buffer_size;
    adev->device.open_output_stream = adev_open_output_stream;
    adev->device.close_output_stream = adev_close_output_stream;
    adev->device.open_input_stream = adev_open_input_stream;
    adev->device.close_input_stream = adev_close_input_stream;
    adev->device.dump = adev_dump;

    *device = &adev->device.common;

    return 0;
}

static struct hw_module_methods_t hal_module_methods = {
    .open = adev_open,
};

struct audio_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = AUDIO_MODULE_API_VERSION_0_1,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = AUDIO_HARDWARE_MODULE_ID,
        .name = "Default audio HW HAL",
        .author = "The Android Open Source Project",
        .methods = &hal_module_methods,
    },
};

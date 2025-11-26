#include <SDL2/SDL.h>
#include <signal.h>
#include <stdio.h>

// gcc audiorecord.c -I/ucrt64/include/SDL2 -Dmain=SDL_main -lmingw32 -lSDL2main -lSDL2
// For Linux:
// export SDL_AUDIODRIVER=alsa
// sudo usermod -a -G audio $USER

/* 查找包含指定子字符串的音频设备名称 */
const char *find_device_name_containing(const char *substring)
{
    int numdevs = SDL_GetNumAudioDevices(1);
    for (int i = 0; i < numdevs; i++)
    {
        const char *device_name = SDL_GetAudioDeviceName(i, 1);
        if (strstr(device_name, substring) != NULL)
            return device_name;
    }
    return NULL;
}

void audio_callback(void *userdata, uint8_t *stream, int len)
{
    printf("audio_callback: %d\n", len);

    static int count = 1024 * 1024 * 10;
    static FILE *fp;
    if (count > 0)
    {
        printf("remaining %d KB\n", count / 1024);
        count -= len;
        if (fp == NULL)
            fp = fopen("audio.pcm", "wb");
        fwrite(stream, 1, len, fp);
        if (count <= 0)
        {
            fclose(fp);
            fp = NULL;
        }
    }
}

SDL_AudioDeviceID open_audio_device(const char *device_name)
{
    SDL_AudioSpec desired_spec, obtained_spec;
    SDL_zero(desired_spec);
    desired_spec.freq = 48000;
    desired_spec.format = AUDIO_S32LSB;
    desired_spec.channels = 1;
    desired_spec.samples = 4096;
    desired_spec.callback = audio_callback;

    SDL_AudioDeviceID device_id = SDL_OpenAudioDevice(device_name, 1, &desired_spec, &obtained_spec, 0);
    if (device_id == 0)
    {
        printf("Cannot open audio device: %s\n", SDL_GetError());
        return -1;
    }

    printf("Actual audio device format:\n");
    printf("  Sample rate: %d Hz\n", obtained_spec.freq);
    printf("  Channels: %d\n", obtained_spec.channels);
    printf("  Format: %s\n", obtained_spec.format == AUDIO_S32SYS ? "32bit PCM" : "Other format");
    printf("  Buffer size: %d samples\n", obtained_spec.samples);
    return device_id;
}

void close_audio_device(SDL_AudioDeviceID device_id)
{
    SDL_CloseAudioDevice(device_id);
}

int audio_sample_task(void *data)
{
    SDL_AudioDeviceID device_id = 0;

    while (1)
    {
        const char *device_name = find_device_name_containing("MicNode");
        // 没找到，关闭已打开的设备
        if (device_name == NULL)
        {
            if (device_id > 0)
            {
                close_audio_device(device_id);
                device_id = 0;
                printf("Device disconnected\n");
            }
        }

        // 找到设备后，尝试打开
        else
        {
            if (device_id <= 0)
            {
                device_id = open_audio_device(device_name);
                if (device_id > 0)
                {
                    SDL_PauseAudioDevice(device_id, 0);
                }
            }
        }

        SDL_Delay(1000);
    }
}

int audio_init(void)
{
    int res = SDL_Init(SDL_INIT_AUDIO);
    if (res < 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return res;
    }

    printf("Current audio driver: %s\n", SDL_GetCurrentAudioDriver());

    SDL_Thread *th = SDL_CreateThread(audio_sample_task, "AudioThread", NULL);

    return 0;
}

void signal_handler(int sig)
{
    printf("Received signal %d, exiting program\n", sig);
    exit(0);
}

int main(int argc, char *argv[])
{
    audio_init();

    signal(SIGINT, signal_handler);

    while (1)
    {
        SDL_Delay(1000);
    }

    return 0;
}

#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <alsa/asoundlib.h>
#include <stdbool.h>

// 录音参数
#define SAMPLE_RATE 48000       // 采样率
#define CHANNELS 1              // 声道数 (1=单声道)
#define BUFFER_SIZE 4096        // 减少缓冲区大小，提高兼容性
#define DEVICE_CHECK_INTERVAL 1000000  // 设备检测间隔（微秒）
#define DEVICE_NAME_PATTERN "MicNode"  // 设备名称模式

// 全局变量
static bool is_init = false;
static bool device_is_open = false;
snd_pcm_t *recording_device = NULL;
char *audio_buffer = NULL;
static pthread_mutex_t device_mutex = PTHREAD_MUTEX_INITIALIZER;  // 设备访问互斥锁

// 查找包含指定模式的录音设备
char* find_micnode_device() {
    int err;
    void **hints;
    void **n;
    char *found_device = NULL;
    
    // 获取录音设备列表
    err = snd_device_name_hint(-1, "pcm", &hints);
    if (err < 0) {
        printf("获取录音设备列表失败: %s\n", snd_strerror(err));
        return NULL;
    }
    
    // 遍历设备列表，查找包含DEVICE_NAME_PATTERN的设备
    for (n = hints; *n != NULL; n++) {
        char *io = snd_device_name_get_hint(*n, "IOID");
        if (io && strcmp(io, "Input") == 0) {
            char *name = snd_device_name_get_hint(*n, "NAME");
            if (name && strstr(name, DEVICE_NAME_PATTERN) != NULL) {
                // 找到匹配的设备，优先选择plughw类型
                if (strstr(name, "plughw") != NULL) {
                    found_device = strdup(name);
                    printf("找到匹配的MicNode设备: %s\n", found_device);
                    free(name);
                    break;
                } else if (found_device == NULL) {
                    // 如果还没有找到设备，保存当前设备
                    found_device = strdup(name);
                }
                free(name);
            }
        }
        if (io) free(io);
    }
    
    snd_device_name_free_hint(hints);
    return found_device;
}
// 初始化ALSA录音设备
int init_alsa_device(const char *device_name, unsigned int sample_rate, int channels) {
    int err;
    snd_pcm_hw_params_t *params;
    snd_pcm_format_t format = SND_PCM_FORMAT_S32_LE;  // 修复：使用正确的宏名称
    snd_pcm_uframes_t period_size = BUFFER_SIZE;  // 修复：使用正确的类型
    unsigned int buffer_time = 500000;  // 500ms的缓冲区
    
    // 打开PCM设备用于录音
    err = snd_pcm_open(&recording_device, device_name, SND_PCM_STREAM_CAPTURE, 0);
    if (err < 0) {
        printf("无法打开录音设备 %s: %s\n", device_name, snd_strerror(err));
        return 0;
    }
    
    // 分配硬件参数对象
    snd_pcm_hw_params_alloca(&params);
    
    // 设置默认参数
    err = snd_pcm_hw_params_any(recording_device, params);
    if (err < 0) {
        printf("无法初始化硬件参数: %s\n", snd_strerror(err));
        return 0;
    }
    
    // 设置访问模式为交错
    err = snd_pcm_hw_params_set_access(recording_device, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0) {
        printf("无法设置访问模式: %s\n", snd_strerror(err));
        return 0;
    }
    
    // 设置格式为32位PCM
    err = snd_pcm_hw_params_set_format(recording_device, params, format);
    if (err < 0) {
        printf("无法设置格式: %s\n", snd_strerror(err));
        return 0;
    }
    
    // 设置声道数
    err = snd_pcm_hw_params_set_channels(recording_device, params, channels);
    if (err < 0) {
        printf("无法设置声道数: %s\n", snd_strerror(err));
        return 0;
    }
    
    // 设置采样率
    err = snd_pcm_hw_params_set_rate(recording_device, params, sample_rate, NULL);
    if (err < 0) {
        printf("无法设置采样率: %s\n", snd_strerror(err));
        return 0;
    }
    
    // 设置缓冲区时间
    err = snd_pcm_hw_params_set_buffer_time_near(recording_device, params, &buffer_time, NULL);
    if (err < 0) {
        printf("无法设置缓冲区时间: %s\n", snd_strerror(err));
        return 0;
    }
    
    // 设置周期大小
    err = snd_pcm_hw_params_set_period_size_near(recording_device, params, &period_size, NULL);
    if (err < 0) {
        printf("无法设置周期大小: %s\n", snd_strerror(err));
        return 0;
    }
    
    // 应用硬件参数
    err = snd_pcm_hw_params(recording_device, params);
    if (err < 0) {
        printf("无法应用硬件参数: %s\n", snd_strerror(err));
        return 0;
    }
    
    // 获取实际设置的参数以显示
    snd_pcm_uframes_t buffer_size;
    snd_pcm_hw_params_get_buffer_size(params, &buffer_size);
    snd_pcm_hw_params_get_period_size(params, &period_size, NULL);
    
    printf("实际录音设备格式:\n");
    printf("  采样率: %u Hz\n", sample_rate);
    printf("  声道数: %d\n", channels);
    printf("  格式: 32bit PCM\n");
    printf("  周期大小: %lu 样本\n", (unsigned long)period_size);  // 修复：使用正确的类型输出
    printf("  缓冲区大小: %lu 样本\n", buffer_size);
    
    // 准备音频流
    err = snd_pcm_prepare(recording_device);
    if (err < 0) {
        printf("无法准备音频流: %s\n", snd_strerror(err));
        return 0;
    }
    
    return 1;
}


// 关闭录音设备
void close_alsa_device() {
    pthread_mutex_lock(&device_mutex);
    if (recording_device != NULL) {
        snd_pcm_close(recording_device);
        recording_device = NULL;
        device_is_open = false;
        printf("录音设备已关闭\n");
    }
    pthread_mutex_unlock(&device_mutex);
}

// 发送音频数据到Dart
void send_audio_to_dart(char* buffer, int len)
{
    printf("发送 %d 字节音频数据到 Dart\n", len);

    static int count = 1024 * 1024 * 10;
    static FILE *fp;
    if (count > 0)
    {
        printf("remaining %d KB\n", count / 1024);
        count -= len;
        if (fp == NULL)
            fp = fopen("audio.pcm", "wb");
        fwrite(buffer, 1, len, fp);
        if (count <= 0)
        {
            fclose(fp);
            fp = NULL;
        }
    }
}

// 音频线程函数
void *audio_thread(void *arg) {

    // 初始化音频缓冲区 (32位 = 4字节/样本，BUFFER_SIZE个样本)
    audio_buffer = (char*)malloc(BUFFER_SIZE * 4 * CHANNELS);
    if (audio_buffer == NULL) {
        printf("无法分配音频缓冲区\n");
        return NULL;
    }
    
    printf("音频线程已启动，开始监听MicNode设备...\n");
    fflush(stdout);
    
    while (true) {

        pthread_mutex_lock(&device_mutex);
        
        if (!device_is_open) {
            pthread_mutex_unlock(&device_mutex);
            // 查找MicNode设备
            char *device_name = find_micnode_device();
            if (device_name != NULL) {
                pthread_mutex_lock(&device_mutex);
                // 再次检查设备是否已打开（可能在等待锁的过程中被其他线程打开）
                if (!device_is_open) {
                    printf("尝试打开设备: %s\n", device_name);
                    if (init_alsa_device(device_name, SAMPLE_RATE, CHANNELS)) {
                        device_is_open = true;
                        printf("设备 %s 已成功打开\n", device_name);
                    } else {
                        printf("无法打开设备: %s\n", device_name);
                    }
                }
                free(device_name);
                pthread_mutex_unlock(&device_mutex);
            } else {
                // 未找到设备，等待一段时间后再次尝试
                usleep(DEVICE_CHECK_INTERVAL);
            }
        } else {
            pthread_mutex_unlock(&device_mutex);
            
            // 从设备读取音频数据
            int ret = snd_pcm_readi(recording_device, audio_buffer, BUFFER_SIZE);
            
            if (ret == -EPIPE) {
                printf("录音设备发生溢出\n");
                // 尝试恢复设备
                pthread_mutex_lock(&device_mutex);
                if (recording_device != NULL) {
                    if (snd_pcm_prepare(recording_device) < 0) {
                        printf("无法恢复设备，关闭连接\n");
                        close_alsa_device();
                    }
                }
                pthread_mutex_unlock(&device_mutex);
            } else if (ret < 0) {
                printf("读取录音数据错误: %s\n", snd_strerror(ret));
                // 发生严重错误，关闭设备并尝试重新连接
                close_alsa_device();
            } else if (ret > 0) {
                // 成功读取数据，计算实际字节数并发送到Dart
                int bytes_read = ret * CHANNELS * 4;  // 32位 = 4字节/样本
                send_audio_to_dart(audio_buffer, bytes_read);
            }
        }

        fflush(stdout);
        // 短暂休眠以减少CPU使用率
        usleep(1);
    }
    
    // 清理资源（理论上不会执行到这里）
    if (audio_buffer != NULL) {
        free(audio_buffer);
        audio_buffer = NULL;
    }
    close_alsa_device();
    
    return NULL;
}

// 初始化音频系统
void audio_init() {
    if (is_init) {
        return;
    }

    // 初始化互斥锁
    pthread_mutex_init(&device_mutex, NULL);
    
    // 创建音频线程
    pthread_t thread;
    pthread_create(&thread, NULL, audio_thread, NULL);
    pthread_detach(thread);  // 线程结束时自动清理资源
    
    is_init = true;
    printf("音频系统已初始化\n");
}


// 清理音频系统资源
void audio_cleanup() {
    pthread_mutex_lock(&device_mutex);
    
    if (audio_buffer != NULL) {
        free(audio_buffer);
        audio_buffer = NULL;
    }
    
    close_alsa_device();
    is_init = false;
    
    pthread_mutex_unlock(&device_mutex);
    pthread_mutex_destroy(&device_mutex);
    
    printf("音频系统已清理\n");
}

void main() {
    audio_init();
    while (true) {
        usleep(1000 * 1000);
    }
}

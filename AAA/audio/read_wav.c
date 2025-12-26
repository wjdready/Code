#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// WAV文件基本结构定义
typedef struct {
    // RIFF块
    char  chunk_id[4];          // "RIFF"
    int   chunk_size;           // 文件大小 - 8
    char  format[4];            // "WAVE"

    // 格式块
    char  subchunk1_id[4];      // "fmt "
    int   subchunk1_size;       // 格式块大小(一般为16)
    short audio_format;         // 音频格式(PCM为1)
    short num_channels;         // 声道数
    int   sample_rate;          // 采样率
    int   byte_rate;            // 字节率 = 采样率 * 声道数 * 每样本位数 / 8
    short block_align;          // 块对齐 = 声道数 * 每样本位数 / 8
    short bits_per_sample;      // 每样本位数
} wav_header_t;

// 打印WAV文件信息，显示总样本数量
void print_wav_info(const wav_header_t* header, int total_samples) {
    printf("=== WAV文件信息 ===\n");
    printf("音频格式: ");
    for (int i = 0; i < 4; i++) {
        if (header->format[i] != '\0') printf("%c", header->format[i]);
    }
    printf("\n");
    printf("音频编码: %d (1 = PCM)\n", header->audio_format);
    printf("声道数: %d\n", header->num_channels);
    printf("采样率: %d Hz\n", header->sample_rate);
    printf("字节率: %d bytes/sec\n", header->byte_rate);
    printf("块对齐: %d\n", header->block_align);
    printf("每样本位数: %d bits\n", header->bits_per_sample);
    printf("总样本数量: %d\n", total_samples);
    printf("====================\n\n");
}

// 跳过非数据块，直到找到data块
int find_data_chunk(FILE* file, int* data_size) {
    char chunk_id[4];
    int chunk_size;
    long current_pos;
    
    // 循环查找data块
    while (1) {
        // 保存当前位置，用于错误处理
        current_pos = ftell(file);
        
        // 读取块ID
        if (fread(chunk_id, sizeof(chunk_id), 1, file) != 1) {
            printf("读取块ID失败\n");
            return 0;
        }
        
        // 读取块大小
        if (fread(&chunk_size, sizeof(chunk_size), 1, file) != 1) {
            printf("读取块大小失败\n");
            return 0;
        }
        
        // 检查是否为data块
        if (strncmp(chunk_id, "data", 4) == 0) {
            *data_size = chunk_size;
            return 1;  // 找到data块
        } else {
            // 跳过这个块的数据部分
            printf("跳过非数据块: ");
            for (int i = 0; i < 4; i++) {
                printf("%c", chunk_id[i]);
            }
            printf(" (大小: %d bytes)\n", chunk_size);
            
            // 跳过数据部分，注意要考虑块大小的奇偶性（根据RIFF规范）
            fseek(file, chunk_size + (chunk_size % 2), SEEK_CUR);
        }
    }
}

// 打印音频数据，使用逗号分隔，不换行，支持16进制模式
void print_audio_data(FILE* file, const wav_header_t* header, int data_size, int samples_to_print, int hex_mode) {
    printf("音频数据 (共打印 %d 个样本)", samples_to_print);
    if (hex_mode) {
        printf(" [16进制模式]");
    }
    printf(":\n");
    
    int first_sample = 1;  // 标记是否为第一个样本
    
    // 根据位深度读取和打印数据
    if (header->bits_per_sample == 8) {
        // 8位数据，通常是无符号的
        uint8_t sample;
        for (int i = 0; i < samples_to_print; i++) {
            for (int ch = 0; ch < header->num_channels; ch++) {
                if (fread(&sample, sizeof(sample), 1, file) != 1) {
                    printf("读取数据失败\n");
                    return;
                }
                
                if (!first_sample) {
                    printf(", ");  // 非首个数据前加逗号
                } else {
                    first_sample = 0;
                }
                
                if (hex_mode) {
                    printf("0x%02X", sample);
                } else {
                    printf("%d", sample);
                }
            }
        }
    } 
    else if (header->bits_per_sample == 16) {
        // 16位数据，通常是有符号的
        int16_t sample;
        for (int i = 0; i < samples_to_print; i++) {
            for (int ch = 0; ch < header->num_channels; ch++) {
                if (fread(&sample, sizeof(sample), 1, file) != 1) {
                    printf("读取数据失败\n");
                    return;
                }
                
                if (!first_sample) {
                    printf(", ");  // 非首个数据前加逗号
                } else {
                    first_sample = 0;
                }
                
                if (hex_mode) {
                    printf("0x%04X", (uint16_t)sample);
                } else {
                    printf("%d", sample);
                }
            }
        }
    } 
    else if (header->bits_per_sample == 24) {
        // 24位数据，有符号
        uint8_t buf[3];
        int32_t sample;
        for (int i = 0; i < samples_to_print; i++) {
            for (int ch = 0; ch < header->num_channels; ch++) {
                if (fread(buf, sizeof(buf), 1, file) != 1) {
                    printf("读取数据失败\n");
                    return;
                }
                // 转换24位数据到32位整数
                sample = (buf[2] << 16) | (buf[1] << 8) | buf[0];
                // 符号扩展
                if (sample & 0x800000) {
                    sample |= 0xFF000000;
                }
                
                if (!first_sample) {
                    printf(", ");  // 非首个数据前加逗号
                } else {
                    first_sample = 0;
                }
                
                if (hex_mode) {
                    printf("0x%06X", (uint32_t)(sample & 0xFFFFFF));
                } else {
                    printf("%d", sample);
                }
            }
        }
    } 
    else if (header->bits_per_sample == 32) {
        // 32位数据，通常是有符号整数
        int32_t sample;
        for (int i = 0; i < samples_to_print; i++) {
            for (int ch = 0; ch < header->num_channels; ch++) {
                if (fread(&sample, sizeof(sample), 1, file) != 1) {
                    printf("读取数据失败\n");
                    return;
                }
                
                if (!first_sample) {
                    printf(", ");  // 非首个数据前加逗号
                } else {
                    first_sample = 0;
                }
                
                if (hex_mode) {
                    printf("0x%08X", (uint32_t)sample);
                } else {
                    printf("%d", sample);
                }
            }
        }
    } 
    else {
        printf("不支持的位深度: %d bits\n", header->bits_per_sample);
        return;
    }
    
    printf("\n");  // 最后换行
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("用法: %s <wav文件路径> [-p 样本数量] [-x]\n", argv[0]);
        printf("  -p 样本数量: 打印指定数量的样本数据\n");
        printf("  -x: 以16进制格式显示数据\n");
        printf("  注意: 没有-p参数时，只显示文件信息，不打印数据\n");
        return 1;
    }
    
    const char* filename = argv[1];
    int samples_to_print = 0;  // 默认不打印任何数据
    int hex_mode = 0;          // 默认十进制模式
    
    // 解析命令行参数
    int i = 2;
    while (i < argc) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            // 处理-p参数，表示要打印的样本数量
            samples_to_print = atoi(argv[i + 1]);
            if (samples_to_print <= 0) {
                printf("错误: 样本数量必须大于0\n");
                return 1;
            }
            i += 2;  // 跳过-p和样本数量值
        } else if (strcmp(argv[i], "-x") == 0) {
            // 处理-x参数，表示以16进制模式显示
            hex_mode = 1;
            i++;
        } else {
            printf("错误: 未知参数: %s\n", argv[i]);
            return 1;
        }
    }
    
    // 以二进制模式打开WAV文件
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("无法打开WAV文件");
        return 1;
    }
    
    // 读取WAV文件头
    wav_header_t header;
    if (fread(&header, sizeof(header), 1, file) != 1) {
        printf("读取WAV文件头失败\n");
        fclose(file);
        return 1;
    }
    
    // 验证是否为WAV文件
    if (strncmp(header.chunk_id, "RIFF", 4) != 0 || 
        strncmp(header.format, "WAVE", 4) != 0) {
        printf("无效的WAV文件格式\n");
        printf("chunk_id: %.4s, format: %.4s\n", header.chunk_id, header.format);
        fclose(file);
        return 1;
    }
    
    // 移动文件指针到fmt块之后的位置
    // 计算fmt块的实际大小，如果大于标准的16字节，需要跳过额外的字节
    if (header.subchunk1_size > 16) {
        long bytes_to_skip = header.subchunk1_size - 16;
        printf("跳过额外的格式信息: %ld bytes\n", bytes_to_skip);
        fseek(file, bytes_to_skip, SEEK_CUR);
    }
    
    // 查找data块
    int data_size;
    printf("正在查找数据块...\n");
    if (!find_data_chunk(file, &data_size)) {
        printf("未找到数据块或文件格式不标准\n");
        fclose(file);
        return 1;
    }
    
    printf("找到数据块，大小: %d bytes\n\n", data_size);
    
    // 计算总样本数量
    int total_samples = data_size / (header.block_align);
    
    // 打印WAV文件信息，包含总样本数量
    print_wav_info(&header, total_samples);
    
    // 只有当指定了-p参数时，才打印音频数据
    if (samples_to_print > 0) {
        // 如果请求的样本数超过实际总数，则调整为总数
        if (samples_to_print > total_samples) {
            printf("警告: 请求的样本数超过文件实际样本数，将打印所有样本\n");
            samples_to_print = total_samples;
        }
        
        // 打印音频数据
        print_audio_data(file, &header, data_size, samples_to_print, hex_mode);
    }
    
    fclose(file);
    return 0;
}

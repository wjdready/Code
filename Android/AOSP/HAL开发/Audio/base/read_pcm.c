#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    FILE *file;
    int32_t sample;
    size_t bytesRead;

    // Check if the file path is provided as a command-line argument
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <path_to_pcm_file>\n", argv[0]);
        return 1;
    }

    // Open the PCM file in binary mode
    file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read samples and print every other one
    uint32_t index = 0;
    int flag = 0;
    int32_t sample_old = 0;
    while ((bytesRead = fread(&sample, sizeof(int16_t), 1, file)) > 0) {

        printf("%d ", sample);
        index ++;
        if (index % 4 == 0) {
            printf("\n");
        }
    }

    printf("\n");
    // Close the file
    fclose(file);
    return 0;
}
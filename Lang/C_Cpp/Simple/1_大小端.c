#include <stdio.h>
#include <stdint.h>

int main(int argc, char const *argv[])
{
    int32_t val = 0x44332211;
    uint8_t *pval = (uint8_t *)&val;

    for (int i = 0; i < 4; i++)
        printf("%02x ", pval[i]);
    printf("\n");

    unsigned int x = 1;
    char *c = (char*)&x;

    if (*c) {
        printf("小端 (Little Endian)\n");
    } else {
        printf("大端 (Big Endian)\n");
    }

    return 0;
}

#include <stdio.h>
#include <stdint.h>

#define BIT(x)          (1 << (x)) 
#define BITS_PER_LONG   32

/*
 * Create a contiguous bitmask starting at bit position @l and ending at
 * position @h. For example
 * GENMASK_ULL(39, 21) gives us the 64bit vector 0x000000ffffe00000.
 */
#define GENMASK(h, l) \
    (((~0UL) - (1UL << (l)) + 1) & (~0UL >> (BITS_PER_LONG - 1 - (h))))


void printbits(uint32_t n)
{
    if(n >= 2) 
        printbits(n / 2);
    
    printf("%d", n % 2);
}

void print_bit(uint32_t n)
{
    printbits(n);
    printf("\n");
}

int main(int argc, char const *argv[])
{
    uint32_t a = 0xAC; 
    print_bit(a);                       // 输出 10101100
    print_bit(a | BIT(1));              // 输出 10101110, 将某一位设为 1
    print_bit(a & (~BIT(2)));           // 输出 10101000, 将某一位设为 0
    print_bit(a & BIT(3));              // 输出 1000, 判断某一位是否为 1, 其实完整应该是 a & BIT(3) == BIT(3)
    print_bit((a & BIT(3)) >> 3);       // 输出 1, 取得某一位的 0, 1 值

    // 同时检查多个位
    int b = (BIT(7) | BIT(2) | BIT(0));
    print_bit(b);                       // 输出 10000101, 创建 0, 2, 7 位掩码
    print_bit(a & b);                   // 输出 10000100, 检查 a 的 0, 2, 7 位之中至少有一位为 1
    print_bit(a & b == b);              // 输出 0, 同时检测 a 的 0 2 7 这 3 位是否都为 1

    // 当然也可以利用 mask 将多个位同时设 0 或 1
    print_bit(a | b);                   // 输出 10101101, 将 a 的 0, 2, 7 位同时设为 1
    print_bit(a & ~b);                  // 输出 101000, 将 a 的 0, 2, 7 位同时设为 0

    // 生成特定范围内的多位 MASK (掩码), 用法和使用 BIT 创建的掩码一致
    print_bit(GENMASK(6, 1));           // 输出 1111110, 生成 1 ~ 6 位范围的掩码, 包含 1, 6 位
    print_bit(GENMASK(3, 1) | BIT(5));  // 输出 101110, 生成 1, 2, 3, 5 位的掩码

    return 0;
}

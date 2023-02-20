#include <stdio.h>

int gvar_init = 84;     // 全局已初始化变量   --> .data 段
int gvar_init_0 = 0;    // 全局初始化为0变量  --> .bss 段
int gvar_uninit;        // 全局未初始化变量   --> .bss 段

// 代码段 --> .text 段
void func(int i)
{
    printf("%d\n", i);
}

int main(int argc, char const *argv[])
{
    static int svar_init = 12;     // 静态已初始化变量   --> .data 段
    static int svar_init_0 = 0;    // 静态初始化为0变量  --> .bss 段
    static int svar_uninit;        // 静态未初始化变量   --> .bss 段
    
    // 放在栈中
    int a = 1;

    // 代码段 --> .text 段
    func(gvar_init + svar_init + a);
    return 0;
}


## Keil 调试技巧
Keil Debug->Function Editor

[官方文档](https://www.keil.com/support/man/docs/uv4cl/uv4cl_df_userfunctions.htm)

```c
// 在嵌入式代码中定义变量和函数可在这里引用
// lastSp 在嵌入式代码中定义为全局变量
FUNC void PrintStack (int len) {

	unsigned int x, i;

	exec("log > MyValues.log");

	x = SP;

	for (i = 0; i < len; i++)
	{
		if (i == 0)
			printf("LR = 0x%08X\tPC=0x%08X\tsp->\t", LR, PC);
		else if (lastSp == x)
			printf("\t\tsp->\t");
		else
			printf("\t\t\t");

		printf("0x%08X\t0x%08X", x, *(unsigned int *)x);
		
		printf("\n");
		x += 4;
	}
	
	lastSp = SP;

	printf("\n");
	exec("log off");
}
```

## 使用栈帧进行代码回溯

[参考](https://stackoverflow.com/questions/33955719/how-unwind-arm-cortex-m3-stack)

添加编译参数 `--use_frame_pointer` 开启栈帧指针

```c
__asm uint32_t getRegisterR11()
{
    mov r0,r11
    BX LR
}

void unwindBacktrace(uint32_t topFp)
{
    uint32_t nextFp = topFp;

    int cur_depth = 0;

    for(int i = 0; i < BACK_TRACE_DEPTH; i++, cur_depth ++)
    {
        uint32_t lr = *((uint32_t*)nextFp) - sizeof(size_t) - 1;
        printf("0x%08X ", lr);

        nextFp = *((uint32_t*)nextFp - 1);
        if (nextFp == 0)
            break;
    }
}
```


## CM Backtrace 原理

ARM cortex-m 在执行 BX 或 BL 指令时，硬件会自动将当前指令的下一个地址保存到 LR 寄存器中，而编译器会在函数调用进入新的函数时，第一件事就是将 LR 寄存器压入栈中。如下图所示:

```
                        低地址
                    +-------------+
                    |             |
                    |             |
                    |             |
                    |-------------|
       +----> SP2-> |             |
       |            |    local    |
       |            |  variables  | 3. SUB SP, SP, #n (将当前SP减去相应的偏移量，为局部变量分配空间)
       |            |             |
  function call     |-------------|
       |            |  used regs  | 2. PUSH Rn-Rm     (将函数中用到的寄存器压入栈中)
       |            |-------------|
       |            |     LR      | 1. PUSH LR        (将 LR 压入栈中， LR 指向调用时调用指令的下一个指令的地址)
       |            |-------------| <----------+
       +---- SP1 -> |             |            |
                    |    local    |            |
                    |  variables  |            |
                    |             |           栈帧
                    |-------------|            |        
                    |  used regs  |            |
                    |-------------|            | 
                    |     LR      |            |
                    |-------------| <----------+
                    |             |
                    |             |
                    +-------------+
                        高地址

```

每次调用都会存一次 LR 寄存器，所以可以通过遍历栈中的 LR 来获取函数调用历史。

由于栈中除了 LR 外还包含着寄存器备份和局部变量，要检查从 SP 开始往下读取到的每一个 uint32_t 是否是 LR, 检查的依据是:

1. LR 总是奇数的
2. LR 所指向的地址是调用指令的下一个地址, 例如

```
0x08000038 BL func2
0x0800003C MOV R0, #1
```

则调用时当前 PC = 0x08000038, 下一个指令的地址是 0x0800003C, 这时 LR (奇数) = 0x0800003C + 1 = 0x0800003D

所以只需检查地址 *(LR - 5) 处的地址的指令是否是 BL 或 BX, 就能判断是否是 LR，从而获取栈中的所有 LR

```c
for (; sp < stack_top; sp += sizeof(size_t))
{
    pc = *((uint32_t *)sp);
    // 检查是否是奇数地址
    if (pc % 2 == 0)
        continue;

    // 转成调用时的地址
    pc = pc - sizeof(size_t) - 1;

    // 检查是否在代码段内
    if (pc < code_start || pc >= code_end)
        continue;

    // 检查地址处的指令是否是 BL 或 BX
    int ins_ok = check_ins(pc);
    if (!ins_ok)
        continue;

    // 确认获取到了一个调用时的地址
    printf("%08X ", pc);
}
```

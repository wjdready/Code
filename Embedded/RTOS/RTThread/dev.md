

# [2023-03-31 08:17:58]

坑点: 配置 STM32H7 SDIO 驱动时官方的 drv_sdio.c 没有加入 H7 的版本, 然后在正点原子的 atk-apollo 开发板驱动例程中找到 drv_sdio, 但是支持 SDMMC1，而我的板子接的是 SDMMC2 无法使用。

但是在这个过程中也了解到一些驱动都是直接操作寄存器的, 比如 drv_sdio.c 直接会使用 SDIO 控制器的地址 SDIO_BASE_ADDRESS, 操作寄存器来完成操作, 这种操作虽然简洁快速, 但是需要对芯片寄存器和驱动原理掌握得很透彻, Linux 中很多芯片厂商得驱动基本都是直接用寄存器操作完成。

但是这基本也只有原厂能对芯片做到如此熟悉，很多其他人写的驱动还都是通过 HAL 库来操作的.

atk-apollo 中的驱动并没有更改官方 drv_sdio.c 驱动，而是在 bsp 中添加自己的 drv_sdio.c, 我认为这是极好的, 当 RT-Thread 中的驱动无法满足板子需要时，再弄出一个板级驱动来适配就可以了。

atk-apollo 中的 drv_sdio.c 只有 SDMMC1 并且 控制器地址 SDIO_BASE_ADDRESS 指向 SDMMC1, 但是当我直接改成 SDMMC2, 并把  SDIO_BASE_ADDRESS 指向 SDMMC2 还是不行, 有一些细微之处可能我不知道.

后来我又再 ART-PI 中找打支持 SDMMC2 的驱动，但是下进去发现识别出 sd0 了, 但是操作文件总是失败, 提示 `wait cmd completed timeout` , 调试了好久也没发现问题，然后再一个偶然的事件突然想起来板子上没画上拉电阻，而在配置 Cubemx 时并没有记得配置上拉电阻，因此把 SCK 和 CMD 引脚配置成上拉便正常工作了，这个问题好像我之前也遇到过，但是没记录下来，现在又踩坑了。看来问题的出现必须每一步都检查到位才行.

# [2023-03-31 08:41:48]

调试驱动时，如果出错可以在 menuconfig 中打开日志功能可能会对发现问题有帮助...

## 实现动态加载模块

在 rtconfig.py 中添加 模块编译相关变量

```py
if PLATFORM == 'gcc':

    ...
    
    STRIP = PREFIX + 'strip'
    M_CFLAGS = CFLAGS + ' -mlong-calls -fPIC '
    M_CXXFLAGS = CXXFLAGS + ' -mlong-calls -fPIC'
    M_LFLAGS = DEVICE + CXXFLAGS + ' -Wl,--gc-sections,-z,max-page-size=0x4' +\
                                    ' -shared -fPIC -nostartfiles -nostdlib -static-libgcc'
    M_POST_ACTION = STRIP + ' -R .hash $TARGET\n' + SIZE + ' $TARGET \n'
```

执行

```sh
rtt.bat scons --target=ua -s

git clone https://github.com/RT-Thread/rtthread-apps.git

cd rtthread-apps
export RTT_ROOT=$PWD/../rt-thread
export BSP_ROOT=$PWD/../bsp/FankeH7B0
rtt.bat scons --app=hello

file hello/hello.mo 
# hello/hello.mo: ELF 32-bit LSB shared object, ARM, EABI5 version 1 (SYSV), dynamically linked, stripped
# 可以看到 生成的 hello/hello.mo 是一个动态连接版本, 仅有 3K 大小
```

装到 SD 卡中执行 hello.mo 后发现没有任何反应, 单步调试后发现需要使用 RT_USING_POSIX_FS, 否则在加载模块时就会自动退出...

```c
struct rt_dlmodule* dlmodule_load(const char* filename)
{
#ifdef RT_USING_POSIX_FS
    int fd = -1, length = 0;
#endif
    rt_err_t ret = RT_EOK;
    rt_uint8_t *module_ptr = RT_NULL;
    struct rt_dlmodule *module = RT_NULL;

#ifdef RT_USING_POSIX_FS
    fd = open(filename, O_RDONLY, 0);
    if (fd >= 0)
    {
        length = lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);

        if (length == 0) goto __exit;

        module_ptr = (uint8_t*) rt_malloc (length);
        if (!module_ptr) goto __exit;

        if (read(fd, module_ptr, length) != length)
            goto __exit;

        /* close file and release fd */
        close(fd);
        fd = -1;
    }
    else
    {
        goto __exit;
    }
#endif

    if (!module_ptr) goto __exit;

    ...
```

还真多亏了单步调试, 否则这个问题怎么也想不到...


# [2023-04-03 13:27:24]

打算适配 rtthread Fanke H7B0 开发板上, 遇到的一些问题:

首先是串口, 我想使用 USART1, 但是配置完后发现一直在串口中断死循环, 好像中断处理函数没有把某个标志位清除掉导致, 调试了很久，后来改成使用 UART4 口得了, 这时 UART4 一配置就成功，难道是 USART 和 UART 还有什么区别, 而官方驱动没适配这种情况？既然 UART4 能用，先不管了。

调试 SDIO 和文件系统, 在使用 Cubemx 配置完引脚和时钟后，出现了之前一样的提示 `wait cmd completed timeout` 但这次连设备 sd0 都没枚举出来，说明应该是另一种类型的错误，一开始以为是时钟频率过高，sd卡不识别，改成 129MHz 还是不行，单步调试后发现根本就没进中断。我还以为 RTThread 会给我们配置好中断，结果其实在 Cubemx 中给中断打勾是有效果的。因为它会生成到 HAL_SD_MspInit 函数中:

```c
/* SDMMC1 interrupt Init */
HAL_NVIC_SetPriority(SDMMC1_IRQn, 0, 0);
HAL_NVIC_EnableIRQ(SDMMC1_IRQn);
/* USER CODE BEGIN SDMMC1_MspInit 1 */
```

板子 SD卡接口没有探测引脚，也就无法热插拔。于是在 fish 中执行 

```c
mount sd0 /sdcard elm
```

# [2023-04-14 09:11:13]

打算开一个线程来闪烁 led 作为工作指示灯, 使用 INIT_BOARD_EXPORT 时发现进不去线程, 只能使用 INIT_APP_EXPORT 初始化的线程才能进入线程, 可能是那时系统都还没初始化，这时候配置线程进去不。

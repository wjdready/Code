
# [2023-04-22 21:18:49]


官方 [sdk](https://github.com/bouffalolab/bl_mcu_sdk) 中包含了 BL616，BL618，BL808 等芯片, 操作流程基本都一样，因此直接参照 BL618 即可。

操作如下:

```
export PATH=$PATH:/c/Users/shino/Desktop/CodeNote/Embedded/Board/Sipeed/M0S/tools/toolchain_gcc_t-head_windows/bin
cd examples/helloworld
make ninja CHIP=bl808 BOARD=bl808dk CPU_ID=m0
make flash CHIP=bl808 COMX=COM71 CPU_ID=m0
```

> Sipeed 的 M1s Dock 板子上有一个 UART 和 OTG 的 TypeC 口, UART 转串口芯片使用 BL702 芯片，该串口接到 BL808 的 UART0，因此直接接串口后即可下载
> 该 USB 转串口芯片引出了两个串口，第一个是用来下载的和调试的，第二个目前不知

下载完成后需要按复位键才能启动，而且默认上电没有直接进入系统, 逆天.


# [2023-04-22 21:53:12]

SDK 的结构如下，使用 cmake 构建

* bsp 板级驱动，里面包含不同板子的驱动，其中 board 中包含了板子 GPIO 的与外设映射的初始化函数，链接文件也包含在 baord 目录下
* components 包含了官方提供的上层组件, 里面也包含了 C 标准库组件
* drivers 包含与芯片相关的各类外设驱动, 也包含 soc 汇编启动文件
* examples 里面包含各种示例


uart 调试口在 board 中初始化完成, board.c 官方的 UART 波特率默认总是使用 2000000

printf 被定向输出到 uart0 口, 即下载口, 在 libc 组件中实现, sdk 直接简单粗暴的这样实现 printf:

```c
int printf(const char *fmt, ...)
{
    char print_buf[1024];
    uint32_t len;
    va_list ap;

    if (console == NULL) {
        return 0;
    }

    va_start(ap, fmt);
    len = vsnprintf(print_buf, sizeof(print_buf), fmt, ap);
    va_end(ap);

    len = (len > sizeof(print_buf)) ? sizeof(print_buf) : len;

    bflb_uart_put(console, (uint8_t *)print_buf, len);

    return 0;
}
```

而 `console` 是一个全局变量, 需要调用 `bflb_uart_set_console` 来设置:

```c
struct bflb_device_s *console = NULL; 

void bflb_uart_set_console(struct bflb_device_s *dev)
{
    console = dev;
}
```

这样做，printf 如果输出超过 1KB 大小就会被截掉了。

vsnprintf 使用了开源实现, vsnprintf_nano 是直接使用了 RTThread 的开源实现

但是 libc 下有 newlib 选项, 需要配置才可使用，使用 newlib 就可以不使用这个简单的 printf 实现, 因为 cmake 中这样写到:

```cmake
if(NOT CONFIG_NEWLIB)
    sdk_library_add_sources(snprintf.c)
    sdk_library_add_sources(sprintf.c)
    sdk_library_add_sources(vsprintf.c)
    sdk_library_add_sources(printf.c)
endif()
```

这时使用的应该就是 newlib 中的标志库实现了.


# [2023-04-22 23:21:55]

## 调试 M1s 板子上的 spi lcd 屏

首先复制 board 中的 bl808dk 改为 m1s_bl808

```shell
export PATH=$PATH:/c/Users/shino/Desktop/CodeNote/Embedded/Board/Sipeed/M0S/tools/toolchain_gcc_t-head_windows/bin
cd examples/helloworld
make ninja CHIP=bl808 BOARD=bl808dk CPU_ID=m0
make flash CHIP=bl808 COMX=COM71 CPU_ID=m0
```

lcd_init
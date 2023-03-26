

# [2023-03-12 19:35:49]

[淘宝链接](https://item.taobao.com/item.htm?spm=a1z09.2.0.0.5b792e8dLTWZSH&id=693716966122&_u=h1jo8mlg023a)

[官方资料 getting-started-with-pico](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) 

[pico-sdk](https://github.com/raspberrypi/pico-sdk.git)

跟着 getting-started-with-pico 来好像还是很快的， 下载比较久，先去完善我的 Arduboy 跨平台...

## 特性

* 双核 Cortex-M0 130MHz
* 256KB SRAM 无内置 flash 需外置, zero 外置 QSPI Flash W25Q16JVUXIQ

# [2023-03-12 19:49:15]

呵呵，由于是 ARM 的缘故，环境出奇的简单，在 MSYS 直接可以运行了...

# [2023-03-17 17:51:00]

```sh
git clone https://github.com/raspberrypi/pico-sdk.git
git clone https://github.com/raspberrypi/pico-examples.git

# 导出构建项目
cd pico-examples
mkdir build
cd build
export PICO_SDK_PATH=../../pico-sdk

# cmake 后会在 build 生成各个 example 目录
cmake .. -G "MSYS Makefiles"

# 然后再进入对于目录进行构建即可
cd blink
make -j4
```

官方提供的 bootloader 默认可以模拟成 U 盘, 这时, 生成的 .uf2 固件可以直接复制进去完成烧写。

**默认 printf 绑定到 UART0**

## 启动流程 

根据查找[相关资料](https://wiki.segger.com/Raspberry_Pi_RP2040)得知，RP2040 启动时首先是加载芯片内部 ROM，该 ROM 会扫描 QSPI 设备，并从地址 0 中加载固件，前面需要有 Magic 来正确识别固件。

官方提供的 [Bootm](https://github.com/raspberrypi/pico-bootrom.git) 很不错，源码也很有借鉴意义。Bootm 启动后模拟 U 盘，当用户将 .uf2 文件拷贝进去后 Bootm 程序会将其烧写到 flash 并在下次启动时直接转跳到该地址。

## test_command

有输入输出的地方就有 command.

发现自带的 Cmake 工具链会自动 GC 掉我的 test_command_section，在 `sdk/src/rp2_common/pico_standard_link/CMakeLists.txt` 中可以看到 

```cmake
# PICO_CMAKE_CONFIG: PICO_NO_GC_SECTIONS, Disable -ffunction-sections -fdata-sections and --gc-sections, type=bool, default=0, advanced=true, group=pico_standard_link
# if (NOT PICO_NO_GC_SECTIONS)
#     target_compile_options(pico_standard_link INTERFACE -ffunction-sections -fdata-sections)
#     target_link_options(pico_standard_link INTERFACE "LINKER:--gc-sections")
# endif()
```

也就是说可以通过在 build 中 cmake .. -DPICO_NO_GC_SECTIONS=1 来禁止 `--gc-sections`。

只是禁止 GC 还是不行，还需要在 `pico_sdk\src\rp2_common\pico_standard_link\memmap_default.ld` 文件的 .text 中添加 test_command_section 段

```ld
. = ALIGN(4);
__start_test_command_section = .;
KEEP(*(test_command_section))
. = ALIGN(4);
__stop_test_command_section = .;
```

总之程序是没问题，就是链接时 test_command_section 段没放好，不知为什么。但目前能运行就先不管了。

> 可以通过查看 .map 来获知 test_command_section 的大小和位置。就酱...


# [2023-03-20 23:23:02]

今天尝试使用 JTAG 下载 bootm，因为之前一直觉得 USB bootloader 也是在 QSPI flash 里的, 但是今天我才发现自己错了，真应该好好阅读官方的[rp2040-datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)

看到 rp2040-datasheet 中的 `2.6. Memory`，其实它内部是集成 16KB 的 ROM 的, 这 16KB 的 ROM 用处很大, 包括

* 初始化硬件
* 从 Flash 启动
* 编程至 Flash
* 支持UF2的USB大容量存储设备
* 快速浮点等实用程序库

该只读存储器提供单周期只读总线访问，并采用专用的AHB-Lite仲裁器，因此可以与其他存储设备同时访问。尝试写入该只读存储器不会产生任何影响(不会产生任何总线故障)。

也就是说 USB bootloader 其实已经集成在芯片内部的了，地址是 0x00000000，官方给的 [example](https://github.com/raspberrypi/pico-bootrom) 也只是示例，实际我们无法再更新 ROM 中的程序了。

不过 JTAG 是可以调试和下载到 QSPI Flash 的, 下载算法直接来自 ROM 了...

使用之前买的 CMSIS-DAP 即可烧写:

openocd.cfg

```cfg
source [find interface/cmsis-dap.cfg]
source [find target/rp2040.cfg]

reset_config none separate

program  build/bootrom.bin 0x10000000 verify
```

如果没有 CMSIS-DAP 也可以使用另外一块 RP2040 来进行烧写, [参考链接](https://github.com/rp-rs/rp2040-project-template/blob/main/debug_probes.md)

直接使用现成的[picoprobe.uf2](https://github.com/raspberrypi/picoprobe/releases/download/picoprobe-cmsis-v1.02/picoprobe.uf2) 或自己编译。这样本身也就相当于有一个 CMSIS-DAP 了...

只需连接 GP2 和 GP3 到 SWCLK 和 SWDIO 即可.


# [2023-03-20 23:50:11]

官方文档写的好详细，有时间研究一下吧... 顺便学一下其整个 Cmake 项目的框架，学习面向寄存器编程，这块小小的芯片可供学习的地方真不少啊...

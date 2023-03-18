

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

只是禁止 GC 还是不行，还需要在 ld 文件的 .text 中添加 test_command_section 段

```ld
. = ALIGN(4);
__start_test_command_section = .;
*(test_command_section)
. = ALIGN(4);
__stop_test_command_section = .;
```

总之程序是没问题，就是链接时 test_command_section 段没放好，不知为什么。但目前能运行就先不管了。

> 可以通过查看 .map 来获知 test_command_section 的大小和位置。就酱...

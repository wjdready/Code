

# 2025-11-11 16:16:42

这几天使用 TinyUSB 库，发现 TinyUSB 真是很不错的，工程中提供的很多例程都是能直接运行起来的，用来验证东西很快

比如手上有个 Art-pi 的板子, 直接使用 tinyusb/hw/bsp\stm32h7/boards/stm32h750_weact 这个板子就可以了

需提前准备以下工具即可 cmake, openocd, arm-none-eabi-gcc, ninja

```sh
python tools/get_deps.py -b stm32h750_weact
cmake -B build -DFAMILY=stm32h7 -DBOARD=stm32h750_weact -DCMAKE_BUILD_TYPE=Debug -G Ninja
cmake --build build
```

openocd.cfg

```python
# 设置调试器和MCU, openocd 已经给我们写好了
source [find interface/stlink.cfg]
source [find target/stm32h7x.cfg]

# 复位设置, 对于 ST-link SWD 模式, 必须设置才能软复位
reset_config none separate

# 下载程序
program  build/audio_test.elf verify

reset
```

对于RP2040 之类的，只需提供 SDK_PATH 即可使用

## usb 音频 (单通道 48000 32bit )

基于 examples/device/audio_test 修改成 48000, 32bit 的即可


#### 调试记录

# 2025-11-14 15:56:31

使用自带的 lib/bsp, lib/mcu 烧录到 STM32F411, 发送生成的正弦波发现有丢数据，并且是连续的丢，很可能是时钟没配置好，SysTick 有问题，根据LED 闪烁, 发现不符合预期。

调整时钟也不行，最后使用 Cubemx + keil 手动移植 tinyusb 就可以了


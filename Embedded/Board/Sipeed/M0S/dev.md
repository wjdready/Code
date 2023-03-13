
# [2023-03-13 11:15:43]

模块官方 [wiki](https://wiki.sipeed.com/hardware/zh/maixzero/m0s/m0s.html)

[官方 MCU SDK](https://github.com/bouffalolab/bl_mcu_sdk)

先下载编译器和官方 SDK 再说：
```sh
pacman -S mingw-w64-ucrt-x86_64-riscv64-unknown-elf-gcc
git clone https://github.com/bouffalolab/bl_mcu_sdk.git _bl_mcu_sdk
```

板子上只有一个 USB 口引出，而且没有 USB 转串口，也就说直接接到的是芯片的 USB 口，那么烧录的话也是通过这个口吧

而且有一个 Boot 按钮，应该让芯片进入烧录模式？
调试的话支持 CLink，CLink 是平头哥单片机用的吧。..

插入电脑后我发现有一个 Clink-Lite 和一个 串口，难到说这个模块本直接就能作为 CLink 本身？这样的话直接再买一个模块就可以调试了吧。..

下载好了, 开始编译:

```
cd examples/helloworld
make ninja CHIP=bl616 CPU_ID=m0
```

发现编译不通过, 提示 `cc1.exe: error: unknown cpu 'e907' for '-mtune'` 怀疑是 MSYS 仓库中的 riscv64-unknown-elf-gcc 无法识别它家的cpu?, 下载它们官方提供的编译器看看吧:

```
git clone https://gitee.com/bouffalolab/toolchain_gcc_t-head_windows.git
export PATH=$PATH:/c/Users/shino/Desktop/CodeNote/Embedded/Board/Sipeed/M0S/tools/toolchain_gcc_t-head_windows/bin
where riscv64-unknown-elf-gcc
```


烧写, 根据前面的理解, 进入烧写模式只需要按下 Boot，然后就会弹出下载的 COM 口, 卧槽， 这太方便了啊。简洁而优雅，不需要外置串口模块。

```
# chip_name should be bl602/bl702/bl616/bl808/bl606p, COMX in Windows, /dev/ttyxxx in Linux
make flash CHIP=bl616 COMX=COM44
```

下载成功, 感叹现在的开发真是越来越方便了，和 ESP32、RaspberryPi Pico 一样的开发编译方式, 开发起来真的很舒服...

虽然是 helloworld 程序，但是发现程序下载进去不打印，可能是通过串口打印的吧，直接跑 usb 的例子看看吧

我发现官方 SDK 的 USB 例子其实也是基于 TinyUSB 的, 不得不佩服 TinyUSB 的强大啊, 可移植性不是一般的高。随便哪个芯片都很很快移植，只要在应用层写完相关协议，处处运行不是梦啊

然后测试了一下 usbd_msc_ram 和 cdc 的, 直接就能运行，太爽了！

进入外设时发现支持的外设真的也挺多的啊：

```
adc  cam  cks  csi  dac  dma  efuse  emac  flash  gpio  i2c  i2s  ir  iso11898  mjpeg  mtimer  psram  pwm_v1  pwm_v2  rtc  sec_eng  spi  timer  uart  usbdev  usbhost  wdg
```

简直就是宝藏芯片啊，功能强大，外围硬件简单，非常适合做尺寸很小的产品。


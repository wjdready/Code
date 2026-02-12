

## FT232H

单通道高速USB转多用途 UART/FIFO/JTAG/SPI/I2C
https://ftdichip.com/products/ft232hl/

FT232HL LQFP48 7x7mm
FT232HQ QFN48  6x6mm

默认为 UART 模式, 其他模式需要外接 EEPROM 芯片 (比如93LC56), 并通过官方工具 FT_PROG 写入配置

FT_PROG 下载工具
https://ftdichip.com/wp-content/uploads/2025/07/FT_Prog_v3.12.70.678-Installer.zip

似乎除了串口模式, 其他模式都是 D2XX Direct 模式, 即通过usb编程进行通信

因此设置 JTAG 模式, 首先在 FT_PROG 中扫描识别出设备后 (如果不识别, 可能需要回退驱动), 选择
Hardware Specific -> Port A -> Driver -> D2XX Direct 然后点击 Programe

然后安装D2XX模式的驱动, 如果用于 vivado 调试则安装 install_digilent.exe 这个驱动, 如果是使用 openocd 则可以用 Zadig 工具安装

========== Openocd 中配置 JTAG 模式 ============

默认引脚配置, 可以在 openocd 中更改配置

FT232H    JTAG
ADBUS0    TCK
ADBUS1    TDO/DI
ADBUS2    TDI/DO
ADBUS3    TMS

```sh
adapter driver ftdi
ftdi vid_pid 0x0403 0x6014
transport select jtag
```

SWD 模式, 需要配置 SWDIO 引脚和 SWD_EN, 参考 interface/ftdi/ft232h-module-swd.cfg 

```sh
# source [find interface/ftdi/ft232h-module-swd.cfg]
# 其引脚配置如下, 可以看到 AD1 和 AD2 都被配置成 SWDIO 因此可以选择接其中任意一个即可:
# FT232H    JTAG       SWD
# ADBUS0    TCK        SWDCLK
# ADBUS1    TDO/DI     SWDIO
# ADBUS2    TDI/DO     SWDIO
# ADBUS3    TMS        N/A
# ADBUS4    (GPIOL0)   /nSRST  optional module reset
# ADBUS5    (GPIOL1)   /nTRST  optional target reset
```


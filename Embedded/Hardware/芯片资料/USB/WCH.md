

## CH347
https://www.wch.cn/downloads/CH347DS1_PDF.html

USB2.0 HS 转 UART/SPI/I2C/JTAG/SWD

官方串口部分驱动: https://www.wch.cn/downloads/CH343SER_EXE.html
SPI/I2C/JTAG/SWD 部分驱动: https://www.wch.cn/downloads/CH341PAR_EXE.html

CH347T 无法同时使用多项功能, 根据引脚配置 (复用, 仅上电时检测, 检测完后会自动切换到默认功能)
CH347F 全部功能在引脚不冲突的前提下，可同时使用

DTR1(PIN10)
RTS1(PIN13)

模式0 (55DA): USB 转高速双串口, 串口0支持全MODEM信号, 串口1支持部分MODEM信号
模式1 (55DB): 厂商驱动模式三合一, USB转高速单串口 + USB转2线I2C + USB 转4线SPI 
模式2 (55DC): HID 免驱模式三合一, USB转高速单串口 + USB转2线I2C + USB转4线SPI
模式3 (55DD): 二合一, USB 转高速单串口 + USB转JTAG接口

* Openocd 支持

最新版 Openocd 已经支持 CH347, 如果不支持需要从源码编译

Windows 下 JTAG 部分驱动使用 Zadig 安装

执行如下配置即可识别

```sh
adapter driver ch347
ch347 vid_pid 0x1a86 0x55dd 
adapter speed 10000
transport select swd
```


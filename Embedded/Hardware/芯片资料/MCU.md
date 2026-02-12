

## 低功耗

#### 1. SF32LB58

https://www.sifli.com/zh-hans/sf32lb58x

2个核高性能 M33 240MHz 带FPU, 可用 2176KB SRAM，34uA/MHz
1个核低功耗 M33 96MHz 带FPU, 可用 1056KB SRAM，15.9uA/MHz

集成电源模块

带 2D 图形加速器，USB2.0 HS, 蓝牙 5.3

芯片规格书: https://downloads.sifli.com/silicon/DS0058-SF32LB58x-Datasheet%20V1p6.pdf
用户手册: https://downloads.sifli.com/user%20manual/UM5801-SF32LB58x-%E7%94%A8%E6%88%B7%E6%89%8B%E5%86%8C%20V0p4p3.pdf

BGA 256 封装 (6mmx8mm)

###### 1.1 具体型号 

* 585V5E56 (个人最爱):
SiP QSPI-NOR Flash: 4 + 4MB
SiP O/HPI-PSRAM：32MB

可以直接使用 585V5E56 芯片, 不需要额外的 FLASH 和 PSRAM 资源也足够，BGA 256 可试试

* SF32LB58 官方模组

链接：https://item.taobao.com/item.htm?id=940017626621&mi_id=0000C2fhzhkHptVn69Y5asIlw-5LlTkOoC1IXLkbLMfqcHQ&spm=a21xtw.29178619.0.0

芯片型号: 根据只有1MB 常备FLASH 和 16 + 16MB, 推测是 586VDD36
模组集成FLASH有: 16MB 或者 128MB NAND 和 4GB eMMC 可选

#### 2. SF32LB52

最大问题是没有 USB2.0 HS, 只有 FS

https://downloads.sifli.com/user%20manual/DS5201-SF32LB52x-%E8%8A%AF%E7%89%87%E6%8A%80%E6%9C%AF%E8%A7%84%E6%A0%BC%E4%B9%A6%20V2p5.pdf

没有 Flash 和 PSRAM 同时合封的型号

###### 具体型号 52JUD6: 

最大合封 16MB PSRAM

#### 3. SF32LB55

https://www.sifli.com/zh-hans/sf32lb55x

https://downloads.sifli.com/silicon/PB0001-SF32LB55x-%E4%BA%A7%E5%93%81%E7%AE%80%E4%BB%8B%20V1p3.pdf

###### 具体型号 SF32LB551U405: 

34µA/MHz @3.3V
11.8µA/MHz @3.3

1.4MB on‐chip SRAM, QFN68, 合封 4MB PSRAM + 4MB FLASH


#### 2. nRF54H20

多个320 MHz Arm Cortex-M33 + RISC-V
2 MB Flash + 1 MB RAM
低功耗蓝牙、LE 音频
高速 USB（480 Mbps）


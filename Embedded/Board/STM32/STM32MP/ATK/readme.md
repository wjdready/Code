


```sh
load mmc 0:1 0XC2000040 uImage
load mmc 0:1 0XC3000000 stm32mp157d-atk.dtb
setenv bootargs "root=/dev/mmcblk1p2 rw console=ttySTM0,115200"
bootm 0XC2000040 - 0XC3000000
setenv bootcmd "load mmc 0:1 0XC2000040 uImage;load mmc 0:1 0XC3000000 stm32mp157d-atk.dtb;setenv bootargs "root=/dev/mmcblk1p2 rw console=ttySTM0,115200";bootm 0XC2000040 - 0XC3000000;"
```

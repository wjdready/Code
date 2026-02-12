
对于 STM32 双 bank 的 MCU 需要另外的 cfg 文件...

例如:

`stm32h7x_dual_bank` 

```
bin\openocd  ^
    -f "interface/stlink.cfg" ^
    -f "target/stm32h7x_dual_bank.cfg"  ^
    -c "reset_config none separate" ^
    -c "flash init"       ^
    -c "init"             ^
    -c "reset halt"       ^
    -c "flash erase_sector 0 0 last" ^
    -c "flash erase_sector 1 0 5" ^
    -c "program myboot.bin 0x08000000 verify" ^
    -c "program myapp.bin 0x08040000 verify" ^
    -c "reset" ^
    -c "shutdown"
```

但其实 program 会帮助你擦除, 只要你 bank 对

# 外置 flash 调试方法

[参考](https://whycan.com/t_7425.html)

但是在下载时擦除失败, 因此目前无法实现下载, 调试还需要通过 Boot 进行识别后才能调试:

现在的方案是通过 uf2_boot 下载程序, uf2_boot 在按下按键后弹出 U 盘模式, 将 uf2 固件复制进去即可下载.

调试的话也需要按下按键, 然后执行 openocd 让程序 uf2_boot 运行, 并通过 flash info 1 来获取信息, 之后再启动 VSCode 之类就可以运行 QSPI Flash 的程序了.

```sh
# 设置使用 QSPI, 在 stm32h7x.cfg 根据会该变量添加外部 flash bank
set QUADSPI 1

source [find interface/stlink.cfg]
source [find target/stm32h7x.cfg]

reset_config srst_only srst_nogate

init

# 复位
reset init

# 开始运行 
resume

# 等待板子运行 1s
sleep 1000

# 暂停运行
halt

# 获取 外部 flash 信息, 让 stmqspi 获知外部 flash 信息
flash info 1
```





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
    -c "program AWA5920/AWA5920Bootloader.bin 0x08000000 verify" ^
    -c "program AWA5920/AWA5920.bin 0x08040000 verify" ^
    -c "reset" ^
    -c "shutdown"
```

但其实 program 会帮助你擦除, 只要你 bank 对



```sh
source [find interface/cmsis-dap.cfg]
source [find target/stm32f1x.cfg]

# reset_config none separate
# reset halt

# ===== 烧录 hex 或 elf =====
program app.hex verify

# ===== 烧录 bin =====
# 整片 flash ctrl 0 sector 0 - last
# flash erase_sector 0 0 last
# program app.bin 0x08000000 verify;

# ===== 调试 =====
# ------- telnet 调试---------
# telnet localhost 4444 
# reset halt
# 载到内存
# load_image stm32f10xLoader.bin 0x20000000 bin
# mdb 0x20000000 128
# reg msp 0x20001108
# reg pc 0x20000419
# step 单条执行
# resume
# ------- 使用 gdb 辅助 -------
# arm-none-eabi-gdb
# target remote localhost:3333
# x/10i $pc - 10 查看反汇编
# stepi 单条执行
# set $pc = 0x08000100
# monitor reset 1
# x/1wx 0x480003f8 查看某个地址值
# set {unsigned}0x480003f8=0x20000201
# set {unsigned}0x480003f8=0x00000201
# 设置某个位
# set {unsigned int}0x480003f8 = ({unsigned int}0x480003f8) | (1 << 25)
# set {unsigned int}0x480003f8 = ({unsigned int}0x480003f8) & ~(1 << 25)
```

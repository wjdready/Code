


sf32lb58.cfg

```sh
source [find interface/cmsis-dap.cfg]
# source [find target/stm32f4x.cfg]

source [find target/swj-dp.tcl]
source [find mem_helper.tcl]

set _CHIPNAME sf32lb58
# set _WORKAREASIZE 0x8000

# -expected-id 0x0be12477 不设置时将自动检测
swj_newdap $_CHIPNAME cpu -irlen 4 -expected-id 0x0be12477
dap create $_CHIPNAME.dap -chain-position $_CHIPNAME.cpu

target create $_CHIPNAME.cpu cortex_m -dap $_CHIPNAME.dap -ap-num 0

# 仅仅到这里就可以调试了, 但是没有设置 flash 驱动相关, 所以只能放在内存中调试
# 实际测试, 使用 stm32f4 的代码, 在 ld 中将 ROM 部分放到内存地址, 然后使用 gdb load 到内存
# gdb 会自动将 pc 指向入口, 这时就可以单步调试了, 在 VScode 中, 似乎 {"text": "load"} 无效,
# 只能在启动后在调试控制台中 -exec load 手动加载 (需要在halt 状态), 然后打断点正常
# ==============================================================

# $_CHIPNAME.cpu configure -work-area-phys 0x20000000 -work-area-size $_WORKAREASIZE -work-area-backup 0
# cortex_m reset_config sysresetreq

# ==============================================================
# reset_config none separate
# program  build_sf32lb58-lcd_n16r32n1_qspi_hcpu/main.elf verify

# add-symbol-file build_sf32lb58-lcd_n16r32n1_qspi_hcpu/main.elf 
# target remote localhost:3333
# monitor reset halt
# 查看寄存器 info reg
# 打断点 break main, b main, b 无参数则为当前 pc 
# 查看断点 info breakpoints, info b
# 清除所有断点 delete breakpoints, d
# 查看内存 x/32bx 0x20001000 

# gdb 中 load 之后, 会自动将 pc 指向入口

# arm-none-eabi-readelf -l main.elf # 查看加载地址
```

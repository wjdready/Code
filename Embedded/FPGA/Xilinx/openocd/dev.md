

# [2023-12-09 09:23:50]

通过观察 vitis 下载 flash 时所执行的步骤, 得出以下基于 xsct.bat 的下载脚本

flash.tcl

```sh
# 利用 vitis 工程 platform 下的 ps7_init.tcl 进行 ARM 核初始化
source ps7_init.tcl
stop
ps7_debug
ps7_init
ps7_post_config

# 确保解锁内存访问
set saved_mode [configparams force-mem-accesses]                  
configparams force-mem-accesses 1 

mrd 0xF8000110
mrd 0xF8000100
mrd 0xF8000120
mrd 0xF8000118
mrd 0xF8000108

# 根据 vitis 下载 flash 时的提示, 这三行主要用于内存映射, 以便 uboot.elf 能够顺利下载
# 因为 uboot.elf 链接时映射到了 0xFFFC0000 位置处, uboot 获取位置也是通过 vitis 提示中得知
# Info:  Remapping 256KB of on-chip-memory RAM memory to 0xFFFC0000.
mask_write 0xF8000008 0x0000FFFF 0x0000DF0D
mwr -force 0xF8000910 0x000001FF
mask_write 0xF8000004 0x0000FFFF 0x0000767B
dow uboot.elf

# 将自己的 BOOT.bin 下载到 DDR
dow -data BOOT.bin 0x1000000

# 内存访问重新恢复原样
configparams force-mem-accesses $saved_mode

puts "OK"

# 这时通过 JTAG UART 进入 uboot 界面
jtagterminal

# 运行 uboot
con
```

这时在 uboot 中执行 sf 进行 flash 烧录

```sh
# 查看 0x1000000 地址处是否被加载 BOOT.bin
md 0x1000000

# 探测 flash 芯片
sf probe

# 擦除 flash, end-addr 根据文件大小确定
# 句法: sf erase <start-addr> <end-addr>
sf erase 0x0 0x400000

# 写入 flash, size 根据文件大小确定
# 句法: sf write <MEM-addr> <FLASH-start-addr> <size>
sf write 0x1000000 0 0x21E308
```

###### 自动烧录

> `jtagterminal -socket` 可以不弹出弹窗然后配合 readjtaguart 在当前脚本完成自动烧录
> ```sh 
> proc send_cmd {sockrt_port data} {
>     set chan [socket localhost $sockrt_port]
>     puts $chan "$data"
>     flush $chan
>     close $chan
> }
> 
> readjtaguart 
> set socket_port [jtagterminal -socket]
> 
> # 运行 uboot
> con
> set_cmd $socket_port "help"
> ```
>
> 但是该方法目前唯一的问题是 socket 通信过程好像不太稳定，命令总是偶发性发不到 uboot, 不知为何


## 使用 openocd 

探索通过 openocd 对 xilinx 进行烧录

openocd.cfg 只需配置进入 debug server 模式

```sh
source [find interface/ftdi/digilent_jtag_smt2.cfg]
reset_config srst_only srst_push_pull
source [find target/zynq_7000.cfg]
```

然后在 telnet 中

```sh
halt
targets zynq.cpu0

# 提供 mask_write 命令, 该文件在下面有定义
source xilinx-tcl.cfg

# 首先映射内存
mask_write 0xF8000008 0x0000FFFF 0x0000DF0D
mwr -force 0xF8000910 0x000001FF
mask_write 0xF8000004 0x0000FFFF 0x0000767B

# 加载 uboot
load_image uboot.elf

# 从指定地址处运行
resume 0xfffc0000

# 2023-12-09 10:36:56
# uboot 已经运行了，但是由于无法知道 uboot 中如何通过 jtaguart 和 openocd 进行通信
# 只能到此为止了, 走向下一步的关键是能够解决以下问题:
# 1. openocd 如何实现自定义下载算法, 从而烧进去然后能够和 openocd 通信? 
#    似乎 openocd flash 相关内容是直接集成在 openocd 源码中
# 2. jtag-uart 到底如何和 openocd 进行通信
```

xilinx-tcl.cfg 文件内容, 参考自: 
https://github.com/analogdevicesinc/production-tests/blob/4e89af003561af2ba9a250d7585a7c2d2105a128/pluto-m2k/config/xilinx-tcl.cfg

```sh
#
# TCL to allow the Xilinx PS7 Init TCL code to run in OpenOCD.
#

proc mrd { args } {
    if {[llength $args] == 0} {
        echo "mrd address \[count \[w|h|b\]\]"
        echo "    Read <count> memory locations starting at <address>.  Defaults to one word."
        return
    }
    set addr [lindex $args 0]
    set count  1
    set bits  32
    if {[llength $args] > 1} {
        set count [lindex $args 1]
        if {[llength $args] > 2} {
            switch [lindex $args 2] {
                w       { set bits 32 }
                h       { set bits 16 }
                b       { set bits  8 }
                default { set bits 32 }
            }
        }
    }
    # mem2array x $bits $addr $count
    set x [read_memory $addr $bits $count]
    set nibbles [expr {$bits / 4}]
    set bytes   [expr {$bits / 8}]
    set result  {}
    set idx 0
    foreach {elmt} $x {
        append result [format "%08x:   %0*x\n" [expr {$addr + $idx * $bytes}] $nibbles $elmt]
        set idx [expr {$idx + 1}]
    }
    return $result
}

proc mask_write { addr mask value } {
    # 获取当前值
    set curval "0x[string range [mrd $addr] end-8 end]"
    set maskedval [expr {$curval & ~$mask}]
    #echo "curval = [format 0x%08x $curval] maskedval = [format 0x%08x $maskedval]"
    set writeval [expr {$maskedval | $value}]
    #echo " $addr <= [format 0x%08x $writeval(0)] ([format 0x%08x $curval]: [format 0x%08x $mask]/[format 0x%08x $value])"
    # array2mem writeval 32 $addr 1
    write_memory $addr 32 $writeval
}

proc mwr { flag addr value } {
    mask_write $addr 0xFFFFFFFF $value
}

proc xilinx_ps7_init { bmode } {
   init
   poll off
   if {$bmode == 1} {
        ftdi set_signal JTAG_BMODE 0
   }
   sleep 250
   targets zynq.cpu0
   poll on
   halt
   ps7_debug
   ps7_init
   ps7_post_config
}

proc configparams {args} {
    return 1
}

proc load_uboot { path bmode } {
   xilinx_ps7_init $bmode
   load_image $path
   resume 0x04000000
   exit
}
```



使用 openocd daplink 调试 rtl8720d, 最开始设置 -ap-num 0, 然后一直报错  Cortex-M PARTNO 0x0 is unrecognized, 网上有使用其他芯片的人也反馈这个问题, 但是都没有解决.

后来又换成 JLink, 则报错 Cortex-M CPUID: 0x0 is unrecognized, 可是使用 JLink GDB Server 是能正常识别并调试的,

本来想着后面用 JLink 调试算了, 后来自己编译 openocd, 打开工程发现最新版本新增了 rtl872xd.cfg, 从这个配置文件中得知 -ap-num 只有设置为 1 或 2 才能正常识别 CPU

而且 1 对应的是 Cortex-M23 核, 2 对应的是 Cortex-M33 核, 看来 PARTNO 0x0 is unrecognized 很可能已经提示 -ap-num 0 设置的有问题了

另外也不是版本的问题, 设置 -ap-num 1 后使用之前旧版本也能识别了

```sh
source [find interface/cmsis-dap.cfg]
# source [find interface/jlink.cfg]

transport select swd

source [find target/swj-dp.tcl]
source [find mem_helper.tcl]

set _CHIPNAME rtl8720d

swj_newdap $_CHIPNAME cpu -irlen 4 -expected-id 0x6ba02477
dap create $_CHIPNAME.dap -chain-position $_CHIPNAME.cpu

target create $_CHIPNAME.cpu.km0 cortex_m -dap $_CHIPNAME.dap -ap-num 1
target create $_CHIPNAME.cpu.km4 cortex_m -dap $_CHIPNAME.dap -ap-num 2

cortex_m reset_config sysresetreq

adapter speed 1000
```


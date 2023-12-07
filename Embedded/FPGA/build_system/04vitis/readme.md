


## 关于 vitis 的使用

首先按照正常流程，先在 GUI 环境下导出 .xsa, 然后打开 vitis，使用刚刚导出的 .xsa 文件创建自定义平台

创建完成后会得到 platform.tcl，就像 system_bd.tcl 一样, 要想自动化创建平台，这正是我们想要的。

现在完全可以删除刚刚创建完的平台文件夹，仅仅保留该文件。在任何想要创建平台的时候，只需在 xsct 中 source 该脚本即可:

```sh
xsct.bat -eval "source platform.tcl"
```

> 当然可能需要稍微修改 platform.tcl 文件以设置导出的目录

在今后如果使用 GUI 对平台进行任何配置，vitis 都会将配置内容追加到平台目录下的 platform.tcl 文件。

为了使 `platform.tcl` 总是最新，我们现在不得不手动管理该文件，即: 每当更改配置时，都要手动将新增内容复制到自己项目中保存的 platform.tcl 文件

## 更新 .xsa

在 vivado 中对平台进行的任何更改都需重新导出 .xsa 文件, 然后再重新放到 vitis 工程下面.

关于 vitis 中 xsa 文件应放于何处，暂且使用查找命令吧 `find ./ -name "*.xsa"`，然后将关键位置处替换后还需重新编译。

如果发现没有奏效，可能重新 `source platform.tcl` 会是一个好办法.

在每次重新生成后都需 vitis 中设置好工作区后导入，选择 `Import Project->Eclipse workspace or zip file`

## git 管理

vitis 目录下 `system_wrapper` 是比较大地，它是 `source platform.tcl` 后生成的内容因此可以直接忽略，而创建的 App 目录下的 `Debug/ 和 _ide/` 文件夹也是会自动生成的, 导入项目会自动生成 `.metadata` 故也没必要放入 git, 其他文件则看情况添加进来更多:

vitis/.gitignore

```
Debug/
_ide/
system_wrapper/
.metadata/
RemoteSystemsTempFiles/
IDE.log
.analytics
```

## 收获

这个过程中发现 flash 没下载成功，竟是平台设计时没有正确配置好 SPI Flash 外设，看来下载用的 fsbl 中的 外设也是和平台设计息息相关。

同时根据 vitis 下载 flash 时的提示信息得出自动化下载脚本如下:

```sh
program_flash -f bin/BOOT.bin -offset 0 -flash_type qspi-x4-single -fsbl bin/fsbl.elf -url TCP:127.0.0.1:3121 
```

另外希望使用 openocd 进行 flash 一直没有进展, 虽然可以进入调试模式，并使用 gdb 进行加载程序到内存了:

```sh
source [find interface/ftdi/digilent_jtag_smt2.cfg]
reset_config srst_only srst_push_pull
source [find target/zynq_7000.cfg]

# https://openocd.org/doc-release/html/PLD_002fFPGA-Commands.html
# pld load 0 led.bit
```

当然 pld 是奏效的，可以方便地将 bit 文件下载到 FPGA


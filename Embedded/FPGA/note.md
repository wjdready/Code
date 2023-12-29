

[ARM MCU](https://zhuanlan.zhihu.com/p/489213515) 

开源项目:

0. 基于 Python cocotb 的芯片验证平台
1. oh 开源硬件设计, 提供各种常用 verilog 模块
2. LiteX 框架提供了一个方便高效的基础设施来创建FPGA内核/SoC，探索各种数字设计架构并创建基于FPGA的完整系统


## LSP

VSCode 上的 verilog 插件支持的不是很好, 较为不错的应该是 `Verilog-HDL/SystemVerilog/Bluespec SystemVerilog` 了, 转跳依赖 ctags

试了 HDL Checker 通过 pip 安装 `hdl-checer` 但是没效果, 虽然是有在后台执行了一些操作, 它会扫描 modelsim 的 vcom, vlog 或者 vivado 的 xvhdl 然后在项目目录构建生成一个文件夹, 用于维护数据.

svlangserve 只需安装即可使用, 但是发现效果依然不佳，文件内变量转跳是可以的，模块间转跳不识别. 但是打开文件后偶尔又可以转跳.

## 学习

通过示例学习:
https://www.chipverify.com/verilog/verilog-arrays-memories


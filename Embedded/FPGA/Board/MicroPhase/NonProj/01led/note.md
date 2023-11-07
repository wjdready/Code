
## 如何设置和关闭一些警告

[参考](https://support.xilinx.com/s/question/0D52E00006iHlrGSAS/turn-off-warnings-of-ip-during-synthesis?language=en_US)

使用 `set_msg_config -help` 获得帮助

Configure how the Vivado tool will display and manage specific messages, based on message ID, string, or severity.

```sh
set_msg_config  [-id <arg>] [-string <args>] [-severity <arg>] [-limit <arg>]
                [-new_severity <arg>] [-suppress] [-regexp] [-quiet] [-verbose]

```

例如 

```sh
# 重新设置综合是抛出的 8-7080 警告为 INFO, 因为该警告对该项目不重要
set_msg_config -id {[Synth 8-7080]} -severity WARNING -new_severity INFO

# 注：要匹配特定的消息ID，请将搜索模式设置为特定于该ID。
# 例如，在以下命令中，第一个命令同时适用于“synth 8-32”和“synth 8-3295”，而第二个命令仅适用于“synth 8-32”：
set_msg_config -id "Synth 8-32" -new_severity "CRITICAL WARNING"
set_msg_config -id {[Synth 8-32]} -new_severity "CRITICAL WARNING"
```

-limit 似乎可以设置某个消息最大容忍度? 极好的


## 一个简单的 flow.tcl

```sh
set script_path [file dirname [file normalize [info script]]]
set output_path ./build
file mkdir $output_path

source $script_path/msg_config.tcl

set_part xc7z010clg400-1

# 设置顶层模块
set top_module_name led

# 引入文件
read_verilog src/hdl/led.v
read_xdc src/xdc/pin.xdc

# 综合并实现
proc do_implement {} {
    global top_module_name
    synth_design -top $top_module_name
    opt_design
    place_design
    phys_opt_design
    route_design
}

# 创建 bit 文件
proc do_create_bitstream {} {
    global output_path top_module_name
    write_bitstream -force $output_path/$top_module_name.bit
}

do_implement
do_create_bitstream

```

## 如何仿真





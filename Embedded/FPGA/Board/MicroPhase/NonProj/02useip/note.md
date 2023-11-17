
创建 IP [参考](http://lucasbrasilino.com/posts/Vivado-project-for-version-control-how-to-package-an-ip-from-sources/) 

```sh
# 读入文件
read_verilog file.v
read_bd file.bd
read_xdc file.xdc

# 创建一个 bd 设计
create_bd_design name

# 设置用户 IP 路径, 这会覆盖 ip 仓库地址
# 当然这个可以在菜单栏中 Windows->IP Catalog 进行管理
set_property ip_repo_paths path [current_project]

# 这时可以使用 concat 追加 IP 路径属性
set_property ip_repo_paths [ \
    concat [get_property ip_repo_paths [current_project]] path \
] [current_project]

# 移除
set_property ip_repo_paths [ \
    lsearch -all -inline [ get_property ip_repo_paths [current_project] ] path \
] [current_project]

make_wrapper -files [get_files  ./bd/mb_ex_1/mb_ex_1.bd] -top
open_bd_design ./bd/mb_ex_1/mb_ex_1.bd

# If the block design does not have the output products generated, 
# generate the output products needed for synthesis and implementation runs
set_property synth_checkpoint_mode None [get_files ./bd/mb_ex_1/mb_ex_1.bd]
generate_target all [get_files ./bd/mb_ex_1/mb_ex_1.bd]

# Export the implemented hardware system to the Vitis environment
write_hw_platform -fixed -force -file ./mb_ex.xsa
```

create_ip

```sh
create_ip  [-vlnv <arg>] -module_name <arg> [-dir <arg>] [-force]
           [-allow_hidden] [-vendor <arg>] [-library <arg>] [-name <arg>]
           [-version <arg>] [-revision <arg>] [-quiet] [-verbose]
```

> 生成 ip 的麻烦所在
> 比较麻烦的问题在于当我自定义一个 custom_ip，该 custom_ip 用到了另一个 anthor_ip, 那么导出的这个 custom_ip 还需要包含引用到的 anthor_ip 实例
> 而调用者无法通过 custom_ip 来生成其依赖的 ip 实例。 这就导致 custom_ip 库必须已经包含 anthor_ip, 而 anthor_ip 可能是库 ip，且很可能很大。
> 为了不将库 ip 也放在一起，只能每次重新生成 custom_ip, 并且不把生成的 custom_ip 引入版本管理，而只把其源码引入。

## 使用 ip 的两种方法

##### 1. create_ip 

在 tcl 中使用 create_ip 动态创建 ip 并配置, 生成 xci 输出到 build, 可使用 gui 生成后复制其生成脚本和配置参数

```sh
create_ip -name mult_gen -vendor xilinx.com -library ip -version 12.0 -module_name mult_ratio -dir $output_path/ip_gen
set_property -dict [list \
    CONFIG.PortAWidth {31} \
    CONFIG.PortBWidth {26} \
    CONFIG.Multiplier_Construction {Use_Mults} \
    CONFIG.PipeStages {6} \
    CONFIG.PortAType {Unsigned} \
] [get_ips mult_ratio]
generate_target all [get_ips]
```


##### 2. import_ip

通过 gui 生成 xci 后复制 xci 到工程然后复用，或者直接使用被人项目中生成的 xci, 使用 import_ip 读入 ip，这会将 xci 复制到 proj 中并进行处理，而我们的 proj 在 build 中
因此这种方法也是非常适合版本管理的。对于生成的 xci ip 芯片不匹配的情况只需 upgrade ip 即可

```sh
set ip_files [glob src/ip/*.xci]
foreach file $ip_files {
    import_ip $file
}
# generate_target all [get_ips]
```
> 需要注意的是不要使用 read_ip，因为那样会在当前 xci 源文件中生成很多临时文件

## 使用 bd 文件

在 gui 中创建 bd 好后，点击 File->Export 从而导出 bd.tcl，使用时只需 soruce bd.tcl 就可以重新生成 bd 文件了

同时 wrapper 也是可以通过脚本动态创建的

```sh
# 添加 bd 和 wrapper
create_bd_design system
source src/system_bd.tcl
make_wrapper -files [get_files $project_dir/$project_name.srcs/sources_1/bd/system/system.bd] -top

set top_wrapper $project_dir/$project_name.srcs/sources_1/bd/system/hdl/system_wrapper.v
add_files -norecurse -fileset $src_fileset $top_wrapper
```

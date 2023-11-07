
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

create_ip -name led -vendor lucasbrasilino.com -library AXIS -version 1.0 -module_name led_0


```sh
if { $::argc != 1 } {
    puts "ERROR: Program \"$::argv0\" requires 1 arguments!\n"
    puts "Usage: $::argv0 <ip_dir>\n"
    exit
}

set ip_dir [lindex $::argv 0]

# IP folder does not exist. Create IP folder
file mkdir ${ip_dir}

# create_ip requires that a project is open in memory.
# Create project but don't do anything with it
create_project -in_memory

create_ip -name floating_point -vendor xilinx.com -library ip -version 7.1 -module_name xil_fdiv -dir ${ip_dir}
set_property -dict [list CONFIG.Component_Name {xil_fdiv} CONFIG.Operation_Type {Divide} CONFIG.Flow_Control {NonBlocking} CONFIG.Has_ACLKEN {true} CONFIG.C_Has_UNDERFLOW {true} CONFIG.C_Has_OVERFLOW {true} CONFIG.C_Has_INVALID_OP {true} CONFIG.C_Has_DIVIDE_BY_ZERO {true} CONFIG.A_Precision_Type {Single} CONFIG.C_A_Exponent_Width {8} CONFIG.C_A_Fraction_Width {24} CONFIG.Result_Precision_Type {Single} CONFIG.C_Result_Exponent_Width {8} CONFIG.C_Result_Fraction_Width {24} CONFIG.C_Mult_Usage {No_Usage} CONFIG.Has_RESULT_TREADY {false} CONFIG.C_Latency {28} CONFIG.C_Rate {1}] [get_ips xil_fdiv]

create_ip -name floating_point -vendor xilinx.com -library ip -version 7.1 -module_name xil_fsqrt -dir ${ip_dir}
set_property -dict [list CONFIG.Component_Name {xil_fsqrt} CONFIG.Operation_Type {Square_root} CONFIG.Flow_Control {NonBlocking} CONFIG.Has_ACLKEN {true} CONFIG.C_Has_INVALID_OP {true} CONFIG.A_Precision_Type {Single} CONFIG.C_A_Exponent_Width {8} CONFIG.C_A_Fraction_Width {24} CONFIG.Result_Precision_Type {Single} CONFIG.C_Result_Exponent_Width {8} CONFIG.C_Result_Fraction_Width {24} CONFIG.C_Mult_Usage {No_Usage} CONFIG.Has_RESULT_TREADY {false} CONFIG.C_Latency {28} CONFIG.C_Rate {1}] [get_ips xil_fsqrt]

create_ip -name floating_point -vendor xilinx.com -library ip -version 7.1 -module_name xil_fma -dir ${ip_dir}
set_property -dict [list CONFIG.Component_Name {xil_fma} CONFIG.Operation_Type {FMA} CONFIG.Add_Sub_Value {Add} CONFIG.Flow_Control {NonBlocking} CONFIG.Has_ACLKEN {true} CONFIG.C_Has_UNDERFLOW {true} CONFIG.C_Has_OVERFLOW {true} CONFIG.C_Has_INVALID_OP {true} CONFIG.Has_A_TUSER {false} CONFIG.A_Precision_Type {Single} CONFIG.C_A_Exponent_Width {8} CONFIG.C_A_Fraction_Width {24} CONFIG.Result_Precision_Type {Single} CONFIG.C_Result_Exponent_Width {8} CONFIG.C_Result_Fraction_Width {24} CONFIG.C_Mult_Usage {Medium_Usage} CONFIG.Has_RESULT_TREADY {false} CONFIG.C_Latency {16} CONFIG.C_Rate {1} CONFIG.A_TUSER_Width {1}] [get_ips xil_fma]

generate_target all [get_ips]

close_project -delete

```
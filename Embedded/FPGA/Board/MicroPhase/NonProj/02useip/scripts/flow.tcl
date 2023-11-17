# 输入选项: gui sim 或 auto (默认)
set mode [lindex $argv 0]

# 路径信息
set script_path [file dirname [file normalize [info script]]]
set output_path ./build
file mkdir $output_path

# 工程信息
set part xc7z010clg400-1
set project_name proj
set project_dir $output_path/$project_name

# 配置报错警告消息
source $script_path/msg_config.tcl

# 创建工程
create_project $project_name $project_dir -part $part -force

# 设置 IP 路径
set_property ip_repo_paths [ concat [get_property ip_repo_paths [current_project]] ../01led ] [current_project]

# 刷新 IP
update_ip_catalog -rebuild -scan_changes

# 创建 IP
# create_ip -name led -vendor xilinx.com -library user -version 1.0 -module_name led_0 -dir $output_path

# 导入 IP
set ip_files [glob src/ip/*.xci]
foreach file $ip_files {
    import_ip $file
}

generate_target all [get_ips]

# 添加 hdl 文件
if {[string equal [get_filesets -quiet sources_1] ""]} {
    create_fileset -srcset sources_1
}
set src_fileset [get_filesets sources_1]
add_files -norecurse -fileset $src_fileset src/hdl/top.v

# 约束文件
read_xdc src/xdc/pin.xdc

# 添加 bd 和 wrapper
create_bd_design system
source src/system_bd.tcl
make_wrapper -files [get_files $project_dir/$project_name.srcs/sources_1/bd/system/system.bd] -top

set top_wrapper $project_dir/$project_name.srcs/sources_1/bd/system/hdl/system_wrapper.v
add_files -norecurse -fileset $src_fileset $top_wrapper

# 添加仿真文件
# if {[string equal [get_filesets -quiet sim_1] ""]} {
#     create_fileset -simset sim_1
# }
# set sim_fileset [get_filesets sim_1]
# set sim_files [glob src/sim/*]
# add_files -norecurse -scan_for_includes -fileset [get_filesets sim_1] $sim_files
# # set_property -name "xsim.compile.xvlog.more_options" -value "-d DATAFILE=${data_file}" -objects $sim_fileset
# # set_property -name "xsim.simulate.runtime" -value "all" -objects $sim_fileset
# set_property -name top -value led_tb -objects $sim_fileset

# 设置顶层模块为 system_wrapper
set_property top top [current_fileset]

update_compile_order -fileset sources_1

if {[string equal $mode "sim"]} {
    # 启动仿真
    launch_simulation
    start_gui
} elseif {[string equal $mode "gui"]} {
    start_gui
} else {
    # 综合
    launch_runs synth_1 -jobs 16
    wait_on_run synth_1

    # 实现并生成 bitstream
    launch_runs impl_1 -to_step write_bitstream -jobs 16
    wait_on_run impl_1
}

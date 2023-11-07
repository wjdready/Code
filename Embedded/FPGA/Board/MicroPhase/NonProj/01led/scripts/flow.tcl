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

# 添加 hdl 文件
if {[string equal [get_filesets -quiet sources_1] ""]} {
    create_fileset -srcset sources_1
}
set src_fileset [get_filesets sources_1]
add_files -norecurse -fileset $src_fileset "src/hdl/led.v"

# 约束文件
read_xdc src/xdc/pin.xdc

# 添加仿真文件
if {[string equal [get_filesets -quiet sim_1] ""]} {
    create_fileset -simset sim_1
}
set sim_fileset [get_filesets sim_1]
set sim_files [glob src/sim/*]
add_files -norecurse -scan_for_includes -fileset [get_filesets sim_1] $sim_files
# set_property -name "xsim.compile.xvlog.more_options" -value "-d DATAFILE=${data_file}" -objects $sim_fileset
# set_property -name "xsim.simulate.runtime" -value "all" -objects $sim_fileset
set_property -name top -value led_tb -objects $sim_fileset

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

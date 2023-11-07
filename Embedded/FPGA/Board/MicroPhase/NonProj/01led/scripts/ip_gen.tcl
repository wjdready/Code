# 路径信息
set script_path [file dirname [file normalize [info script]]]
set output_path ./build
file mkdir $output_path

# 工程信息
set part xc7z010clg400-1
set project_name proj
set project_dir $output_path/$project_name

# 加载文件
read_verilog src/hdl/led.v

# 打包 ip, -root_dir 设置打包生成的地址, 以当前工程作为 ip 地址有助于调试
ipx::package_project -import_files -force -root_dir .

# 设置 ip 属性和信息
set_property vendor              {xilinx.com}            [ipx::current_core]
set_property library             {user}                  [ipx::current_core]
set_property taxonomy            {{/demo}}               [ipx::current_core]
set_property vendor_display_name {shino}                 [ipx::current_core]
set_property company_url         {xilinx.com}            [ipx::current_core]
set_property supported_families  {
    virtex7    Production \
    qvirtex7   Production \
    kintex7    Production \
    kintex7l   Production \
    qkintex7   Production \
    qkintex7l  Production \
    artix7     Production \
    artix7l    Production \
    aartix7    Production \
    qartix7    Production \
    zynq       Production \
    qzynq      Production \
    azynq      Production \
    zynquplus  Production
} [ipx::current_core]

# 配置接口
# ipx::remove_memory_map {s_axi} [ipx::current_core]
# ipx::add_memory_map {s_axi} [ipx::current_core]
# ipx::associate_bus_interfaces -busif s_axi -clock sys_clk [ipx::current_core]
# ipx::associate_bus_interfaces -busif m_axi -clock sys_clk [ipx::current_core]
# set_property slave_memory_map_ref {s_axi} [ipx::get_bus_interface s_axi [ipx::current_core]]

# Associate AXI/AXIS interfaces and reset with clock
# set aclk_intf [ipx::get_bus_interfaces ACLK -of_objects [ipx::current_core]]
# set aclk_assoc_intf [ipx::add_bus_parameter ASSOCIATED_BUSIF $aclk_intf]
# set_property value M_AXIS:S_AXIS:S_AXI $aclk_assoc_intf
# set aclk_assoc_reset [ipx::add_bus_parameter ASSOCIATED_RESET $aclk_intf]
# set_property value ARESETN $aclk_assoc_reset

# Set reset polarity
# set aresetn_intf [ipx::get_bus_interfaces ARESETN -of_objects [ipx::current_core]]
# set aresetn_polarity [ipx::add_bus_parameter POLARITY $aresetn_intf]
# set_property value ACTIVE_LOW ${aresetn_polarity}

# 保存 IP
# ipx::check_integrity [ipx::current_core]
# ipx::archive_core [concat $design.zip] [ipx::current_core]
ipx::save_core [ipx::current_core]
close_project

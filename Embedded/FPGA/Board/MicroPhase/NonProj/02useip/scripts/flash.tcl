
# 要烧录的文件路径
set bit_file build/proj/proj.runs/impl_1/system_wrapper.bit

open_hw_manager
connect_hw_server 
open_hw_target
current_hw_device [get_hw_devices xc7z010_1]
set_property PROGRAM.FILE $bit_file [current_hw_device]
program_hw_device [current_hw_device]


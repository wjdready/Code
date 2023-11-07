

# WARNING: [Synth 8-7080] Parallel synthesis criteria is not met
# 这一警告表明，该设计的规模不够大，无法从并行合成中受益。因此，不会运行并行流
# 因此该设计中可忽略
set_msg_config -id {[Synth 8-7080]} -severity WARNING -new_severity INFO

# WARNING: [DRC ZPS7-1] PS7 block required: The PS7 cell 
# must be used in this Zynq design in order to enable correct default configuration.
# 这一警告表明 PS7 核心没有初始化模块，该设计不需要用到 PS7 ARM 核心，可忽略
set_msg_config -id {[DRC ZPS7-1]} -severity WARNING -new_severity INFO


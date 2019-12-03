
# ======设置==========

project_name = make_test
build_dir = ./build

include_dir += ./func

# source_dir += ./func/ok
source_dir += ./func

cc = gcc
FLAGS = -I$(include_dir)

# ====================

# 设置生成目标
target = $(build_dir)/$(project_name).out

# 获取源文件列表
src += $(wildcard ${source_dir}/*.c)

# 获取目标文件列表,附加了目录
obj = $(patsubst %.c,${build_dir}/%.o,$(notdir ${source_dir})) 
	

# $(target) : $(obj) 
# 	$(cc) $^ -o $@

$(obj) : $(src) | $(build_dir)
	$(cc) $(FLAGS) -c $< -o $@

$(build_dir) :
	@mkdir $(build_dir)

clean:
	@echo $(src)
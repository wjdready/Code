
# KBuild 将输出文件保存在当前工作目录中, 这不需要与内核源代码树的根路径匹配
# 
# 例如，你可以这样做
#
#  cd /dir/to/store/output/files; make -f /dir/to/kernel/source/Makefile
#
# 如果你想要将文件输出到另一个文件夹，这里有用两种语法来指定它
#
# 1) O=
# 使用 "make O=dir/to/store/output/files/"
#
# 2) 设置 KBUILD_OUTPUT 变量
# 将环境变量 KBUILD_OUTPUT 设置为指向输出目录
# export KBUILD_OUTPUT=dir/to/store/output/files/; make
#
# O= 赋值优先于 KBUILD_OUTPUT 环境变量

ifeq ("$(origin O)", "command line")
  KBUILD_OUTPUT := $(O)
endif

# 如果设置了输出路径，则创建该路径并获取其绝对路径
ifneq ($(KBUILD_OUTPUT),)
# Make 的内置函数，如$(abspath...), $(realpath...), 无法展开 shell 特殊字符 '~', 
# 我们在这里使用一种有点乏味的方式
abs_objtree := $(shell mkdir -p $(KBUILD_OUTPUT) && cd $(KBUILD_OUTPUT) && pwd)
$(if $(abs_objtree),, \
     $(error failed to create output directory "$(KBUILD_OUTPUT)"))

# $(realpath ...) resolves symlinks
abs_objtree := $(realpath $(abs_objtree))
else
abs_objtree := $(CURDIR)
endif # ifneq ($(KBUILD_OUTPUT),)

this-makefile := $(lastword $(MAKEFILE_LIST))
abs_srctree := $(realpath $(dir $(this-makefile)))

all:
	echo $(abs_objtree)
	echo $(this-makefile)
	echo $(abs_srctree)


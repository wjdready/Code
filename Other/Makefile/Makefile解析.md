---
mytags: myblog
title: Makefile解析
date: 2023-01-18 00:03:56
categories: [构建工具, Makefile]
tags: [构建工具]
---

本文主要介绍Makefile的常用功能
<!-- more -->

#### 最简约的
Makefile 文件如下
```
SRCS = main.c src/add.c

OBJS = $(SRCS:%.c=%.o)

CFLAGS = -Iinc

all: $(OBJS)
	CC $(CFLAGS) $(OBJS) -o a.exe

%.o: %.c
	CC $(CFLAGS) -c $< -o $@ 

clean:
	rm $(OBJS)
```

> 只要列出详细的源文件路径, OBJ 直接从 SRC 替换 .c 得来, 然后直接在以OBJ作为依赖, 假设已经得到所有 OBJ文件, 即可直接使用 CC 进行链接即可, 而 OBJ 列表中的内容则由 %.o: %.c 一个一个匹配生成. 最后清除所有中间文件也十分的简单, 直接 rm $(OBJS) 即可.



#### 改进版, 把输出中间文件都放到指定目录
Makefile 文件如下
```
SRCS = main.c src/add.c

OBJS = $(addprefix build/, $(SRCS:%.c=%.o))

CFLAGS = -Iinc

all: $(OBJS)
	CC $(CFLAGS) $(OBJS) -o build/a.exe

build/%.o: %.c
	@mkdir -p $(dir $@)
	CC $(CFLAGS) -c $< -o $@ 

clean:
	rm -fR build
```

> 使用addprefix命令将src列表转obj时在每一项添加build/, 当然, 在推导时不要忘了前缀 build/%.c:%.o

#### 更进一步, 自动扫描源文件
Makefile 文件如下
```
SRCS +=  $(wildcard src/*.c *.c)

OBJS = $(addprefix build/, $(SRCS:%.c=%.o))

CFLAGS = -Iinc

all: $(OBJS)
	CC $(CFLAGS) $(OBJS) -o build/a.exe

build/%.o: %.c
	@mkdir -p $(dir $@)
	CC $(CFLAGS) -c $< -o $@ 

clean:
	rm -fR build
```

> 使用 wildcard 命令可以帮我们自动收集指定目录下的源文件, 就不需要一个一个输入了

#### 多文件构建
这里以 LVGL 模拟器仿真工程为例, Makefile 文件如下
```
LVGL_DIR = .
LVGL_DIR_NAME = lvgl

include lv_drivers/lv_drivers.mk
include lvgl/lvgl.mk
include lv_examples/lv_demo.mk

CSRCS +=  $(wildcard main/*.c *.c)

INCS += -I. -Imain -Ilvgl 

OBJS = $(addprefix build/, $(CSRCS:%.c=%.o))

CFLAGS += -DLV_CONF_INCLUDE_SIMPLE -DLV_LVGL_H_INCLUDE_SIMPLE -DLV_DEMO_CONF_INCLUDE_SIMPLE $(INCS)

all: $(OBJS)
	CC $(CFLAGS) $(OBJS) -o a.exe -lSDL2 -lm -mwindows

build/%.o: %.c
	@mkdir -p $(dir $@)
	CC $(CFLAGS) -c $< -o $@ 

clean:
	rm -fR build
```
> 需要用到3个模块, lv_drivers、lvgl、lv_examples 而这些模块的编译信息都在模块的根目录下.mk 文件里, 比如 lv_drivers.mk 文件内容如下：
> ```makefile
>  LV_DRIVERS_DIR_NAME ?= lv_drivers
> 
>  CSRCS += $(wildcard $(LVGL_DIR)/$(LV_DRIVERS_DIR_NAME)/*.c)
>  CSRCS += $(wildcard $(LVGL_DIR)/$(LV_DRIVERS_DIR_NAME)/wayland/*.c)
>  CSRCS += $(wildcard $(LVGL_DIR)/$(LV_DRIVERS_DIR_NAME)/indev/*.c)
>  CSRCS += $(wildcard $(LVGL_DIR)/$(LV_DRIVERS_DIR_NAME)/gtkdrv/*.c)
>  CSRCS += $(wildcard $(LVGL_DIR)/$(LV_DRIVERS_DIR_NAME)/display/*.c)
> ```
> 它帮我们导入了该模块必要的 C 文件, 添加到全局变量 CSRCS 中, 同理 lvgl, lv_drivers 也一样, 我们只要 include 该make文件就可以了, 源文件有了, 头文件路径怎么解决呢？LVGL 的方案是在模块根目录有一个头文件, 该头文件 里面 #include 了必要的头各种必要的头文件, 因此要用到该模块时的任何函数接口, 只要 #include 模块的根头文件就可以了.  这样我们写程序只要关注自己的头文件路径就可以了。

#### Cubemx 生成的 STM32 项目
先列出所有源文件，再根据源文件生成目标文件列表, 对目标文件自动推导即可完成。但是需要注意的是, 这里的目标文件缺失了源文件的路径信息, 因此 `$(BUILD_DIR)/%.o : %.c` 这一句中, make 推导时 `%`.o 已经是非完整路径的文件名, 这导致相应的需求文档中 `%.c` 也没有路径信息, 那么make如何确定是哪个源文件生成的呢？没错, `vpath %.c $(sort $(dir $(SOURCES)))`  起到了关键作用, 他将告诉make在哪些目录查找源文件。
```
BUILD_DIR = build

# 1. 手工生成源文件列表
SOURCES += src/add.c
SOURCES += src/main.c

# 2. 源文件列表 -> 目标文件列表, 并且目标文件重定向到 build 
OBJECTS += $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.c=.o)))
# 3. 设置 vpath, 对所有源文件的 '所在目录' 进行注册
vpath %.c $(sort $(dir $(SOURCES)))

# 4. $(OBJECTS) 作为需求, make 将自动推导如何生成
all: $(OBJECTS)
	cc $(OBJECTS)

$(BUILD_DIR)/%.o : %.c | $(BUILD_DIR)
	cc -c $< -o $@ 

$(BUILD_DIR) :
	@mkdir $@

clean :
	rm -rf build
```
> make 自动推导原理, 先看目标文件是否匹配, 然后看需求文件是否匹配, 如果目标文件匹配OK，将目标文件的 % 带到需求文件中去，如果有文件能够匹配, 则可以顺利生成。

#### LVGL 官方 Example 
其原理是利用 shell 命令 find 查找来获取所有源文件列表, 然后利用源文件生成目标文件列表，目标文件都放到build目录下，而且对源文件的目录信息进行保留。因为源文件和目标文件的名称只相差一个 build目录, 因此直接使用 `$(BUILD_DIR)/%.o : %.c` 让 make 自行推导生成。关于保留目录信息，只需对目标文件所在文件夹进行 -p 的 mkdir 即可实现。
```
SRC_DIR = src
BUILD_DIR = build

# 1. 递归查找 'SRC_DIR' 下所有源文件
SOURCES 		:= $(shell find $(SRC_DIR) -type f -name '*.c' -not -path '*/\.*')
# 2. 源文件列表 -> 目标文件列表, 并且目标文件重定向到 build, SOURCES 是包含路径的
OBJECTS    	:= $(addprefix $(BUILD_DIR)/, $(SOURCES:.c=.o))

# 3. $(OBJECTS) 作为需求, make 将自动推导如何生成
all: $(OBJECTS)
	@echo $(OBJECTS)

$(BUILD_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	cc -c $< -o $@ 

clean :
	rm -rf build
```
> 值得注意的是, 上面的 SRC_DIR 变量是可以多目录指定的, 并且还可以精确控制参加编译的目录。
> 可以看到，这里不需要指定 vpath 是因为需求文件的路径是完整的, 也就是顺利匹配的。

# 多 Makefile 文件

## include

有很多项目的构建框架都是使用 include 的方式来实现 SDK 与应用分离, 其原理是在应用代码处的 Makefile 通过 include SDK_PATH 下的 Makefile 框架来提供统一的构建功能，从而实现在应用目录下进行的代码编译。

### 1) TinyUSB

TinyUSB 的每一个 example 就相当于一个完整的工程项目，每个 example 的 Makefile 基本内容如下:

```makefile
include ../../make.mk

INC += \
	src \
	$(TOP)/hw \

# Example source
EXAMPLE_SOURCE += $(wildcard src/*.c)
SRC_C += $(addprefix $(CURRENT_PATH)/, $(EXAMPLE_SOURCE))

include ../../rules.mk
```

在开头处添加一个 include 来导入一些环境和变量, 然后对给出当前工程的相关编译信息, 这里主要是向变量 INC 中添加头文件路径，向 SRC_C 中添加源文件信息， 这其中包含了 main 函数。

最后在结尾处添加一个 include 来完成真正的编译。
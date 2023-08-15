
CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++

ARCH = -mcpu=cortex-m3 -mthumb
LDFLAGS = $(ARCH) -lc -lrdimon -Tlink.ld -Wl,--gc-sections -lsupc++ -lstdc++
CFLAGS = $(ARCH) -g 

INCLUDE_DIR = -I./include

AMS_SRCS = src/start.s
C_SRCS = 			\
	src/main.c 		\
	src/init.c 		\
	src/backtrace.c \
	src/cm3.c		\
	src/thread.c

CXX_SCRS = src/demo.cpp

OBJS += $(addprefix build/, $(AMS_SRCS:%.s=%.o))
OBJS += $(addprefix build/, $(C_SRCS:%.c=%.o))
OBJS += $(addprefix build/, $(CXX_SCRS:%.cpp=%.o))

default: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -Wl,-Map=build/output.map,--cref -o build/demo.elf

build/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) $(INCLUDE_DIR) -c $< -o $@

qemu:
	qemu-system-arm -M lm3s6965evb -kernel build/demo.elf -nographic -semihosting -S -s
# stm32vldiscovery

clean:
	rm build -rf

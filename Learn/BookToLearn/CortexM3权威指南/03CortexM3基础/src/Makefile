
CC = arm-none-eabi-gcc

ARCH = -mcpu=cortex-m3
LDFLAGS = $(ARCH) -lc -lrdimon -Tlink.ld -Wl,--gc-sections
CFLAGS = $(ARCH) -g 

# AMS_SRCS = 入栈和出栈测试.s
AMS_SRCS = 操作pc指针.s
# AMS_SRCS = temp.s
C_SRCS = main.c

OBJS += $(addprefix build/, $(AMS_SRCS:%.s=%.o))
OBJS += $(addprefix build/, $(C_SRCS:%.c=%.o))

default: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -Wl,-Map=build/output.map,--cref -o build/demo.elf

build/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

qemu:
	qemu-system-arm -M lm3s6965evb -kernel build/demo.elf -semihosting -S -s
# stm32vldiscovery

clean:
	rm build -rf


CC = arm-none-eabi-gcc

ARCH = -mcpu=cortex-m3 -mthumb
LDFLAGS = $(ARCH) -lc -lrdimon -Tlink.ld -Wl,--gc-sections
CFLAGS = $(ARCH) -g 

AMS_SRCS = src/cm3.s
C_SRCS = src/main.c src/init.c src/backtrace.c

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
	qemu-system-arm -M lm3s6965evb -kernel build/demo.elf -nographic -semihosting -S -s
# stm32vldiscovery

clean:
	rm build -rf

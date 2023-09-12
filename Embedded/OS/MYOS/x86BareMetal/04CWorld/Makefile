CC = gcc
LD = ld
CPOY = objcopy 
AS = as

INCLUDE += -Iinclude
ASFLAG = --32 -g -O0
CFLAGS = -m16 -ffreestanding -fno-PIE -nostartfiles -nostdlib -g -O0 $(INCLUDE)
LDFLAGS = -Tlink.ld -m elf_i386 -g

C_SRCS = src/main.c
AS_SRCS = src/start.s

OBJS += $(addprefix build/, $(AS_SRCS:%.s=%.o))
OBJS += $(addprefix build/, $(C_SRCS:%.c=%.o))

all: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o build/os.elf
	$(CPOY) -O binary build/os.elf build/os.bin

build/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAG) $< -o $@

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

boot/bootsect.s: boot/bootsect.S
	$(CC) $(INCLUDE) -E $< -o $@

qemu:
	qemu-system-i386 -M pc -drive file=build/os.bin,index=0,if=floppy,format=raw

debug: all
	qemu-system-i386 -M pc -drive file=build/os.bin,index=0,if=floppy,format=raw -S -s

clean:
	rm -rf build boot/bootsect.s

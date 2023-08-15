CC = gcc
LD = ld
CPOY = objcopy 
AS = as

ASFLAG = --32 -g
AS_SRCS = src/start.s
CFLAGS = -m32 -ffreestanding -fno-PIE -nostartfiles -nostdlib -g
LDFLAGS = -Tlink.ld -m elf_i386 -g
C_SRCS = src/main.c

OBJS += $(addprefix build/, $(AS_SRCS:%.s=%.o))
OBJS += $(addprefix build/, $(C_SRCS:%.c=%.o))

all: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o build/os.elf
	$(CPOY) -O binary build/os.elf build/os.bin

iso: all
	cp build/os.elf root
	grub-mkrescue -o build/bootable.iso root

build/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAG) $< -o $@

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

qemu: iso
	qemu-system-i386 -cdrom build/bootable.iso

debug: iso
	qemu-system-i386 -kernel build/os.elf -S -s

clean:
	rm -rf build

CC = gcc
LD = ld
CPOY = objcopy 
AS = as

ifeq ($(AS),as)
    ASFLAG = --32 -g
	AS_SRCS = src/start.s
else
# nasm 似乎无法 debug
	ASFLAG = -f elf32 -g
	AS_SRCS = src/start_nasm.s
endif

CFLAGS = -m16 -ffreestanding -fno-PIE -nostartfiles -nostdlib -g
LDFLAGS = -Tlink.ld -m elf_i386 -g
C_SRCS = 

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

qemu:
	qemu-system-i386 -M pc -drive file=build/os.bin,format=raw 

debug:
	qemu-system-i386 -M pc -drive file=build/os.bin,format=raw -S -s

clean:
	rm -rf build

CC = gcc
CFLAGS = -Wall -Wextra -e efi_main -nostdlib -fno-builtin -Wl,--subsystem,10

all:
	$(CC) $(CFLAGS) main.c -o main.efi

qemu: main.efi OVMF.fd
	mkdir -p image/EFI/BOOT
	ln -sf ../../../main.efi image/EFI/BOOT/BOOTX64.EFI
	qemu-system-x86_64 -nographic -bios OVMF.fd -drive file=fat:rw:image,media=disk,format=raw

OVMF.fd:
	wget http://downloads.sourceforge.net/project/edk2/OVMF/OVMF-X64-r15214.zip
	unzip OVMF-X64-r15214.zip OVMF.fd
	rm OVMF-X64-r15214.zip

clean:
	rm -f main.efi OVMF.fd
	rm -rf image


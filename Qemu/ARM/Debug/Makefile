
CC = arm-none-eabi-gcc

ARCH = -march=armv7-a -marm -msoft-float

LDFLAGS = $(ARCH) -Tlink.ld -Wl,--gc-sections

CFLAGS = $(ARCH) -Wall  -Werror   -gdwarf-2 -O0 -g 

default:
	@mkdir build -p
	$(CC) -c  $(CFLAGS)   main.c -o build/main.o
	$(CC) -c  $(CFLAGS)   start.s -o build/start.o
	$(CC) build/start.o  build/main.o $(LDFLAGS) -o build/demo.elf

qemu:
	qemu-system-arm -M vexpress-a9 -kernel build/demo.elf -S -s


clean:
	rm build -rf

globalclean:
	make clean

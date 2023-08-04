
CC = arm-none-eabi-gcc

ARCH = -mcpu=cortex-m3

LDFLAGS = $(ARCH) -lc -lrdimon -Tlink.ld -Wl,--gc-sections

CFLAGS = $(ARCH) -g 

default:
	@mkdir build -p
	$(CC) -c  $(CFLAGS)   main.c -o build/main.o
	$(CC) -c  $(CFLAGS)   start.s -o build/start.o
	$(CC) build/start.o  build/main.o $(LDFLAGS) -Wl,-Map=build/output.map,--cref -o build/demo.elf

qemu:
	qemu-system-arm -M lm3s6965evb -kernel build/demo.elf -semihosting -S -s
# stm32vldiscovery

clean:
	rm build -rf

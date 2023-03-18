
CC = arm-none-eabi-gcc

LDFLAGS = -mcpu=cortex-m7 -lc -lm  -lrdimon -Tlink.ld -Wl,--gc-sections

CFLAGS = -mcpu=cortex-m7 -g 

default:
	@mkdir build -p
	$(CC) -c  $(CFLAGS)   main.c -o build/main.o
	$(CC) -c  $(CFLAGS)   start.s -o build/start.o
	$(CC) build/start.o  build/main.o $(LDFLAGS) -Wl,-Map=build/output.map,--cref -o build/demo.elf

openocd:
	make default && openocd

clean:
	rm build -rf


CC = arm-none-eabi-gcc

LDFLAGS = -mcpu=cortex-m7 -Tlink.ld -Wl,--gc-sections  -lc -lm -lnosys  -specs=nano.specs -Wl,-Map=output.map

CFLAGS = -mcpu=cortex-m7 -g 

default:
	@mkdir build -p
	$(CC) -c  $(CFLAGS)   main.c -o build/main.o
	$(CC) -c  $(CFLAGS)   start.s -o build/start.o
	$(CC) build/start.o  build/main.o $(LDFLAGS) -o build/demo.elf 


openocd:
	make default && openocd

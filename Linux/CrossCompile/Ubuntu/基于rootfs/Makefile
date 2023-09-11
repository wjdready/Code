
CC = aarch64-linux-gnu-gcc
CXX = aarch64-linux-gnu-g++

ROOTFS = /home/shino/code/Linux/source_debug/_rootfs

CFLAG += $(LIBS) $(INCLUDES) 
CFLAG += --sysroot=$(ROOTFS)

all:
	$(CC) demo.c $(CFLAG)  -lSDL2 -o sdl2demo

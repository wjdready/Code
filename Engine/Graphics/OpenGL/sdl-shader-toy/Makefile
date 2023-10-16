
SRCS += main.c draw.c init.c file.c exts.c object.c

LIBS += `pkg-config sdl2 --libs --static` -lopengl32 -static

CFLAGS += -Wall $(LIBS)

all:
	gcc $(SRCS) $(CFLAGS)


BUILD_DIR = build
PROJECT_NAME = app

SRCS += add.c main.c
CC = cl
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:%.c=%.obj))

CFLAGS += -nologo
LDFLAGS += -nologo

ifndef BUILD_DIR_EXISTS
$(shell mkdir $(BUILD_DIR) -p)
BUILD_DIR_EXISTS := 1
endif

all: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -link -out:$(BUILD_DIR)/$(PROJECT_NAME).exe

$(BUILD_DIR)/%.obj: %.c
	$(CC) -c $(CFLAGS) -Fo:$@ $< 

clean:
	rm -rf build > /dev/null

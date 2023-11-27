
OUT_DIR ?= .
BUILD_DIR = build
PROJECT_NAME = add

SRCS += add.c
CC = cl
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:%.c=%.obj))

CFLAGS += -nologo
LDFLAGS += -nologo

ifndef BUILD_DIR_EXISTS
$(shell mkdir $(BUILD_DIR) -p)
$(shell mkdir $(OUT_DIR) -p)
BUILD_DIR_EXISTS := 1
endif

all: $(OBJS)
	lib $(OBJS) -out:$(OUT_DIR)/$(PROJECT_NAME).lib
	rm $(BUILD_DIR) -rf

$(BUILD_DIR)/%.obj: %.c
	$(CC) $(CFLAGS) -c -Fo:$@ $< 

clean:
	rm -rf $(BUILD_DIR) *.exp *.dll *.lib > /dev/null

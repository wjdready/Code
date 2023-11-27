
BUILD_DIR = build
PROJECT_NAME = app

SRCS += main.c
CC = cl
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:%.c=%.obj))

LIBS += add.lib

CFLAGS += -nologo
LDFLAGS += -nologo -libpath:lib

ifndef BUILD_DIR_EXISTS
$(shell mkdir $(BUILD_DIR) -p)
BUILD_DIR_EXISTS := 1
endif

all: $(OBJS) $(addprefix lib/, $(LIBS))
	$(CC) $(OBJS) -link $(LDFLAGS) $(LIBS) -out:$(BUILD_DIR)/$(PROJECT_NAME).exe

$(BUILD_DIR)/%.obj: %.c
	$(CC) -c $(CFLAGS) -Fo:$@ $< 

$(addprefix lib/, $(LIBS)):
	make -C $(basename $(notdir $@)) OUT_DIR=$(PWD)/lib

clean:
	rm -rf build > /dev/null

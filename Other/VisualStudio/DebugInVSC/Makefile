
BUILD_DIR = build
PROJECT_NAME = main

SRCS += main.c
CC = cl
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:%.c=%.obj))

LIBS += 
INCLUDE_DIR += 

CFLAGS += -nologo -wd4819 -Zi -EHsc -DEBUG -I $(INCLUDE_DIR)
LDFLAGS += -nologo -DEBUG

ifndef BUILD_DIR_EXISTS
$(shell mkdir $(BUILD_DIR) -p)
BUILD_DIR_EXISTS := 1
endif

all: $(OBJS)
	$(CC) $(OBJS) -link $(LDFLAGS) $(LIBS) -out:$(BUILD_DIR)/$(PROJECT_NAME).exe

$(BUILD_DIR)/%.obj: %.c
	$(CC) -c $(CFLAGS) -Fo:$@ $< 

clean:
	rm -rf build > /dev/null

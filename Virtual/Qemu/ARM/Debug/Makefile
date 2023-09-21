BUILD_DIR = build
TARGET = $(BUILD_DIR)/demo.elf
LINK_FILE = target/link.ld

CC = arm-none-eabi-gcc

C_SRCS += $(wildcard src/*.c *.c)

AS_SRCS += $(wildcard src/*.s *.c)

OBJS += $(addprefix $(BUILD_DIR)/, $(notdir $(C_SRCS:.c=.o)))
OBJS += $(addprefix $(BUILD_DIR)/, $(notdir $(AS_SRCS:.s=.o)))
vpath %.c $(sort $(dir $(C_SRCS)))
vpath %.s $(sort $(dir $(AS_SRCS)))

ARCH = -march=armv7-a -marm -msoft-float

LDFLAGS = $(ARCH) -T$(LINK_FILE) -lc -lm  -lrdimon -Wl,--gc-sections

CFLAGS = $(ARCH) -Wall  -Werror   -gdwarf-2 -O0 -g

default: $(TARGET)
	@echo "Output $(TARGET)"

$(TARGET): $(OBJS)
	@$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)
	@ echo LD $<

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ 
	@echo CC $<

$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ 
	@echo AS $<

$(BUILD_DIR) :
	@mkdir $@

qemu:
	qemu-system-arm -M vexpress-a9 -kernel $(TARGET) -S -s

clean:
	rm build -rf

globalclean:
	make clean

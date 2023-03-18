TARGET = awa_pc
BUILD_DIR = build

SRCS +=  $(wildcard src/*.c port/*.c test/*.c)
SRCS +=  cJSON/cJSON.c

OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.c=.o)))
vpath %.c $(sort $(dir $(SRCS)))

CFLAGS += -Isrc -IcJSON -Iport -Itest
CFLAGS += -g -O0 -Werror -Wall

LDFLAGS += -lpthread -static

$(BUILD_DIR)/$(TARGET): $(OBJS)
	@gcc $(OBJS) $(LDFLAGS) -o $@ 
	@echo Output $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@gcc $(CFLAGS) -c $< -o $@
	@echo CC $<

run:
	cd build && start awa_pc.exe

$(BUILD_DIR) :
	@mkdir $@

clean:
	rm -fR build

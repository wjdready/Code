
######################################
# target
######################################
TARGET = demo

HAL_PATH = ../drivers/STM32H7xx_HAL

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES +=  \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c \
	$(HAL_PATH)/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c \


C_SOURCES += \
	src/system_stm32h7xx.c \
	src/main.c \
	src/test_command.c

# ASM sources
ASM_SOURCES =  \
	src/start.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m7

# fpu
FPU = -mfpu=fpv5-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32H750xx


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
	-Isrc \
	-I$(HAL_PATH)/STM32H7xx_HAL_Driver/Inc \
	-I$(HAL_PATH)/STM32H7xx_HAL_Driver/Inc/Legacy \
	-I$(HAL_PATH)/CMSIS/Device/ST/STM32H7xx/Include \
	-I$(HAL_PATH)/CMSIS/Include

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = target/link.ld

# libraries
LIBS = -lc -lm  -lrdimon
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -specs=rdimon.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)


openocd:
	openocd -f ./target/openocd.cfg

# *** EOF ***

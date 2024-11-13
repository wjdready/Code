# ------------>切换 page 0 --------------
i2cset -y 1 0x18 0x00 0x00

# 进行复位
i2cset -y 1 0x18 0x01 0x01

# PLL 默认关闭, CODEC_CLKIN 时钟直接来自于 MCLK pin

# DAC 设置, 需满足
# CODEC_CLKIN = NDAC * MDAC * DOSR * DAC_FS
# 2.8MHz < DOSR * DAC_FS < 6.2MHz
# MDAC * DOSR / 32 ≥ RC

# MCLK = 25.576M = 1 * 4 * 128 * 48000
# Power up the NDAC divider with value 1
i2cset -y 1 0x18 0x0b 0x81

# Power up the MDAC divider with value 4
i2cset -y 1 0x18 0x0c 0x84

# Program the DOSR of DAC to 128
i2cset -y 1 0x18 0x0d 0x00
i2cset -y 1 0x18 0x0e 0x80

# 设置音频格式 I2S, 32Bit, BCLK WCLK 从ad芯片输出
# i2cset -y 1 0x18 0x1b 0x3c
i2cset -y 1 0x18 0x1b 0x30

# Set the DAC Mode to PRB_P1 RC = 8
i2cset -y 1 0x18 0x3c 0x8


# ------------ ADC 部分 ------------
# Power up NADC divider with value 1
i2cset -y 1 0x18 0x12 0x81

# Power up MADC divider with value 2
i2cset -y 1 0x18 0x13 0x84

# Program OSR for ADC to 128
i2cset -y 1 0x18 0x14 0x80

# Select ADC PRB_R1
i2cset -y 1 0x18 0x3d 0x01

# ---------------------------------


############# my setting #############

# BCLK 分频 8, (24.576k / (48000 * 32 * 2))
i2cset -y 1 0x18 0x1e 0x88

# Audio Data in is routed to Audio Data out. (Works only when WCLK is configured as input.)
# i2cset -y 1 0x18 0x1d 0x20

# Stereo ADC output is routed to Stereo DAC input
# i2cset -y 1 0x18 0x1d 0x10

# 自动生成 BCLK 和 WCLK ?
# i2cset -y 1 0x18 0x1d 0x04

# ADC WCLK 和 DAC WCLK 不同?
# i2cset -y 1 0x18 0x20 0x02

############ end of my setting ############


# ------------>切换 page 1 -----------
i2cset -y 1 0x18 0x00 0x01

# Disabled weak connection of AVDD with DVDD
# 从而使用 AVDD
i2cset -y 1 0x18 0x01 0x08

# 使能模拟部分电源控制, 使能内部 LDO 到 AVDD
i2cset -y 1 0x18 0x02 0x01

# Set the REF charging time to 120ms
i2cset -y 1 0x18 0x7b 0x03

# HP soft stepping 设置，可在通电时实现最佳弹出性能，使用的 Rpop 为 6k (N = 6)，soft step = 0
i2cset -y 1 0x18 0x14 0x25

# Set the Input Common Mode to 0.9V and Output Common Mode for Headphone to Input Common Mode
i2cset -y 1 0x18 0x0a 0x00

# Route Left DAC to HPL
i2cset -y 1 0x18 0x0c 0x08

# Route Right DAC to HPR
i2cset -y 1 0x18 0x0d 0x08

# Set the DAC PTM mode to PTM_P3/4
i2cset -y 1 0x18 0x03 0x00
i2cset -y 1 0x18 0x04 0x00

# Unmute HPL driver and Set the HPL gain to 0dB
i2cset -y 1 0x18 0x10 0x00

# Unmute HPL driver and Set the HPR gain to 0dB
i2cset -y 1 0x18 0x11 0x00

# Power up HPL and HPR drivers
i2cset -y 1 0x18 0x09 0x30

# Wait for 2.5 sec for soft stepping to take effect...

# ------------ ADC 部分 ------------
# Select ADC PTM_R4
i2cset -y 1 0x18 0x3d 0x00

# Set MicPGA startup delay to 3.1ms
i2cset -y 1 0x18 0x47 0x32

# Route IN1L to LEFT_P with 20K input impedance
i2cset -y 1 0x18 0x34 0x80

# Route Common Mode to LEFT_M with impedance of 20K
i2cset -y 1 0x18 0x36 0x80

# Route IN2L to RIGHT_P with input impedance of 20K
i2cset -y 1 0x18 0x37 0x02

# Route Common Mode to RIGHT_M with impedance of 20K
i2cset -y 1 0x18 0x39 0x80

# Unmute Left MICPGA, Gain selection of 6dB to make channel gain 0dB
# Register of 6dB with input impedance of 20K => Channel Gain of 0dB
i2cset -y 1 0x18 0x3b 0x0c

# Unmute Right MICPGA, Gain selection of 6dB to make channel gain 0dB
# Register of 6dB with input impedance of 20K => Channel Gain of 0dB
i2cset -y 1 0x18 0x3c 0x0c

# ---------------------------------

############# my setting #############

# IN1L is routed to HPL
# i2cset -y 1 0x18 0x0C 0x04


############ end of my setting ############

sleep 3

# ------------>切换 page 0 -----------
i2cset -y 1 0x18 0x00 0x00

# Power up the Left and Right DAC Channels with route the Left Audio digital data to
# Left Channel DAC and Right Audio digital data to Right Channel DAC
i2cset -y 1 0x18 0x3f 0xd4

# Unmute the DAC digital volume control
i2cset -y 1 0x18 0x40 0x00

# ------------ ADC 部分 ------------
# Power up Left and Right ADC Channels
i2cset -y 1 0x18 0x51 0xc0

# Unmute Left and Right ADC Digital Volume Control
i2cset -y 1 0x18 0x52 0x00

# ------------>切换 page 1 -----------
# i2cset -y 1 0x18 0x00 0x01

# Enable Class-D mode for HPL output
# i2cset -y 1 0x18 0x03 0xC0

# # Enable Class-D mode for HPR output
# i2cset -y 1 0x18 0x04 0xC0

# ------------>切换 page 0 -----------
# i2cset -y 1 0x18 0x00 0x00

# adb push adc_init.sh /system/bin

# 所有寄存器
# 第 0 页

# 生成Beep
# i2cset -y 1 0x18 0x47 0xA0
# i2cset -y 1 0x18 0x48 0x80
# i2cset -y 1 0x18 0x49 0xff

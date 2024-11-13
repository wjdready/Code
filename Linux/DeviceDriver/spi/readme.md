



## GPIO 模拟 SPI

[参考](https://www.jianshu.com/p/f43fdaf05bf8)

配置内核

```sh
CONFIG_SPI_GPIO=y
# 这个作为依赖会被自动开启
# CONFIG_SPI_BITBANG=y
# 用于测试
CONFIG_SPI_SPIDEV=y
```

设备树配置

```c
// 全志平台, 已验证
spi_gpio1: spi_gpio {
    status = "okay";
    compatible = "spi-gpio";
    pinctrl-names = "default";
    ranges;
    #address-cells = <0x1>;
    #size-cells = <0x0>;

    sck-gpios 	= <&pio PG 6 0>;
    miso-gpios 	= <&pio PG 7 0>;
    mosi-gpios 	= <&pio PG 8 0>;
    cs-gpios 	= <&pio PG 9 0>;

    num-chipselects = <1>;
    spidev@0x00 {
        status = "okay";
        compatible = "spidev";
        spi-max-frequency = <1000000>;
        reg = <0>;
    };
};

// RK 平台 GPIO SPI
spi_gpio1: spi_gpio {
    status = "okay";
    compatible = "spi-gpio";
    pinctrl-names = "default";
    ranges;
    #address-cells = <0x1>;
    #size-cells = <0x0>;

    cs-gpios 	= <&gpio1 RK_PC0 0>;
    sck-gpios 	= <&gpio1 RK_PC1 0>;
    mosi-gpios 	= <&gpio1 RK_PC2 0>;
    miso-gpios 	= <&gpio1 RK_PC3 0>;

    num-chipselects = <1>;
};
    
// 添加一个 oled 设备
&spi_gpio1 {
    ssd1306@0 {
        compatible = "solomon,ssd1306";
        reg = <0>;
        spi-max-frequency = <1000000>;
        buswidth = <8>;
        debug = <1>;
        height = <64>;
        width = <128>;
        fps = <30>;
        reset-gpios = <&gpio1 RK_PC4 1>;
        dc-gpios = <&gpio1 RK_PC5 0>;
        led-gpios = <&gpio1 RK_PD0 0>;
        rotate = <0>;
        bgr;
    };
};
```

当然如果有挂多个设备，则需要 `num-chipselects = <n>;`, 同时提供 `cs-gpios` 引脚数组，可全局搜索设备树比如 `num-chipselects =` 查看参考例子

模拟 spi 速度有上限，即使 device 设置 `spi-max-frequency` 很高也没用，对于接彩色屏幕来说刷新会很慢, 对黑白色的 oled 来说还好

# [2024-11-13 15:05:50] 坑点

GPIO_ACTIVE_LOW 实际是 1, GPIO_ACTIVE_HIGH 实际是 0, 一开始想当然了，导致一开始调全志的 GPIO 模拟时总是显示不出来

## spi 测试

kernel 自带验证工具 `kernel/tools/spi`

编译技巧, 在内核构建环境的 Makefile 中添加一个目标, 然后通过 `make -C kernel/tools/spi` 进行编译，这样调用的编译器就会是和内核编译的编译器相同

```sh
# 回环测试
spidev_test -D /dev/spidev2.0 -v

# 当然也可以指定发送内容或发送文件
spidev_test -D /dev/spidev2.0 -p "Hello World \x01\x02\x03\x04" -v
```

## 开启 FBCON

```sh
CONFIG_FRAMEBUFFER_CONSOLE=y
# 依赖 
VT=y
FB=y

# 然后再启动参数中添加即可输出到屏幕
console=tty0
```


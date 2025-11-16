

```c
void lcd_set_windows(struct mipi_dbi *dbi, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    mipi_dbi_command(dbi, 0x2a, (x1 >> 8) & 0xff, x1 & 0xff, (x2 >> 8) & 0xff, x2 & 0xff);
    mipi_dbi_command(dbi, 0x2b, (y1 >> 8) & 0xff, y1 & 0xff, (y2 >> 8) & 0xff, y2 & 0xff);
    mipi_dbi_command(dbi, 0x2c);
}

int mipi_dbi_write_data(struct mipi_dbi *dbi)
{
    int i = 0;
    static uint8_t clear_buf[240*320*2];

    lcd_set_windows(dbi, 0, 0, 239, 239);

    for(i = 0; i < sizeof(clear_buf); i++) {
        clear_buf[i] = 0xe0;    // 红色
    }

	gpiod_set_value_cansleep(dbi->dc, 1);
	return mipi_dbi_spi_transfer(dbi->spi, 0, 8, clear_buf, sizeof(clear_buf));
}
```

```c
&spi0 {
    display@0{
        status = "okay";
        compatible = "adafruit,yx240qv29";
        reg = <0>;
        spi-max-frequency = <32000000>;
        dc-gpios = <&gpio2 RK_PA4 GPIO_ACTIVE_HIGH>;
        reset-gpios = <&gpio2 RK_PA5 GPIO_ACTIVE_HIGH>;
        rotation = <0>;

        // backlight = <&backlight>;
    };
}
```


另外 tftfb 配置如下, dc-gpios 匹配不上，需要改成 dc 服了

fb_test 可以刷屏，但是 fbcon 怎么输出到 lcd 不知

```c
&spi0 {
    status = "okay";
	pinctrl-names = "default";
	pinctrl-0 = <&spi0m0_pins>;
    pinctrl-names = "default";
	#address-cells = <1>;
	#size-cells = <0>;
    ili9341@0 {
        compatible = "ilitek,ili9341";
        reg = <0>;
        spi-max-frequency = <50000000>;
        rotate = <270>;
        bgr;
        fps = <30>;
        buswidth = <8>;
        reset = <&gpio2 5 GPIO_ACTIVE_LOW>;
        dc = <&gpio2 4 GPIO_ACTIVE_LOW>;
        debug = <1>;
    };
};
```

buildroot 使能 QT

```sh

# 记得加入示例 BR2_PACKAGE_QT5BASE_EXAMPLES

#QT 配置
export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/qt/plugins
export QT_QPA_PLATFORM=linuxfb:tty=/dev/fb0
#export QT_QPA_FONTDIR=/lib/fonts

#TSLIB 配置
export TSLIB_CONSOLEDEVICE=none
export TSLIB_FBDEVICE=/dev/fb0
export TSLIB_TSDEVICE=/dev/input/event2
export TSLIB_TSEVENTTYPE=input
export TSLIB_CONFFILE=/etc/ts.conf
export TSLIB_CALIBFILE=/etc/pointercal
export TSLIB_PLUGINDIR=/usr/lib/ts
export QT_QPA_GENERIC_PLUGINS=tslib
```



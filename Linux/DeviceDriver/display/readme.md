

## 1. fbtft

## 2. drm

### 2.1 tinydrm

#### 2.2 新的驱动 tft lcd 方法

https://blog.csdn.net/CNflysky/article/details/125171176

使能内核模块, CONFIG_DRM_PANEL_MIPI_DBI, 然后在对应的 spi 下添加 panel 节点

```py
&spi1 {
    panel: panel@0 {
        compatible = "panel-mipi-dbi-spi";
        reg = <0>;
        spi-max-frequency = <32000000>;

        width-mm = <20>;
        height-mm = <20>;

        reset-gpios = <&gpio0 RK_PA6 0>;
        dc-gpios = <&gpio0 RK_PA5 0>;
        write-only;

        timing: panel-timing {
            hactive = <240>; // 横向分辨率
            vactive = <320>; // 纵向分辨率
            hback-porch = <0>;
            vback-porch = <0>;

            clock-frequency = <0>;
            hfront-porch = <0>;
            hsync-len = <0>;
            vfront-porch = <0>;
            vsync-len = <0>;
        };
    };
}
```

使用 https://github.com/notro/panel-mipi-dbi.git 工具生成屏幕的初始化序列，并将其添加到文件系统 /lib/firmware 中

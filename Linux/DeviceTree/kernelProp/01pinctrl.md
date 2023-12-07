

参考:
https://github.com/mykhani/device-tree-guide/tree/master#soc-specific-dtsi-file

rv1106 mini 定义 dts

```lua
/dts-v1/;

#include "rv1106.dtsi"

/ {

    model = "WJD Mini RV1106"; 
	compatible = "rockchip,rv1106";
};

&fiq_debugger {
	rockchip,irq-mode-enable = <1>;
	status = "okay";
};
```

> fiq_debugger 通过串口进行调试

引入 `rv1106-evb.dtsi` 开发模式, 使能 `fiq_debugger` 已经在 `rv1106-evb.dtsi` 中定义

```lua
/dts-v1/;

#include "rv1106.dtsi"
#include "rv1106-evb.dtsi"

/ {

    model = "WJD FIQ RV1106"; 
	compatible = "rockchip,rv1106";
};
```

## 使用 sdmmc 

```lua
/dts-v1/;

#include "rv1106.dtsi"
#include "rv1106-evb.dtsi"
/ {
    model = "WJD SDMMC RV1106"; 
    compatible = "rockchip,rv1106";

    chosen {
		bootargs = "rootwait";
	};
};

&sdmmc {
	max-frequency = <50000000>;
	no-sdio;
	no-mmc;
	bus-width = <4>;
	cap-mmc-highspeed;
	cap-sd-highspeed;
	disable-wp;
	pinctrl-names = "default";
	pinctrl-0 = <&sdmmc0_clk &sdmmc0_cmd &sdmmc0_det &sdmmc0_bus4>;
	status = "okay";
};
```

> chosen 节点会被追加到 u-boot 中的 bootargs，但如果 u-boot 中有相同参数会被覆盖。这里 `rootwait` 确保 sd 卡被正确挂载后才识别根文件系统


## 音频接口


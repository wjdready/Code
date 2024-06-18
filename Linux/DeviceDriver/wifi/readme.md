


## RV1106 wifi 调试

参考 https://www.cnblogs.com/armsom/articles/17449208.html

用的是 ap6398s 模块， WIFI 芯片是博通的 bcm4359c0，其驱动位置是 drivers/net/wireless/rockchip_wlan/rkwifi/bcmdhd

其中 wlan-platdata 匹配的是 rfkill 驱动，rfkill 和 bcmdhd 进行配合。但是 rfkill 驱动要先加载进来，bcmdhd 后加载才能有效，因为 bcmdhd 依赖 rfkill 提供的一个获取 irq 的函数.

```c
/ {
	model = "Luckfox Pico Max";
	compatible = "rockchip,rv1103g-38x38-ipc-v10", "rockchip,rv1106";

	sdio_pwrseq: sdio-pwrseq {
		compatible = "mmc-pwrseq-simple";
		clocks = <&cru CLK_RTC_32K>;
		pinctrl-names = "default";
		pinctrl-0 = <&wifi_enable_h &wifi_32k>;
		reset-gpios = <&gpio3 RK_PD2 GPIO_ACTIVE_LOW>;
	};

	wireless_wlan: wireless-wlan {
		compatible = "wlan-platdata";
		rockchip,grf = <&grf>;
		wifi_chip_type = "ap6398s";
		pinctrl-names = "default";
		pinctrl-0 = <&wifi_host_wake_irq>;
		WIFI,host_wake_irq = <&gpio3 RK_PD3 GPIO_ACTIVE_HIGH>;
		status = "okay";
	};

    wireless-bluetooth {
		compatible = "bluetooth-platdata";
		clocks = <&cru CLK_RTC_32K>;
		uart_rts_gpios = <&gpio1 RK_PD0 GPIO_ACTIVE_LOW>;
		pinctrl-names = "default", "rts_gpio";
		pinctrl-0 = <&uart5m1_rtsn>;
        pinctrl-1 = <&uart5_gpios>;
		BT,reset_gpio    = <&gpio3 RK_PD0 GPIO_ACTIVE_HIGH>;
		BT,wake_gpio     = <&gpio1 RK_PC6 GPIO_ACTIVE_HIGH>;
		BT,wake_host_irq = <&gpio1 RK_PC7 GPIO_ACTIVE_HIGH>;
		status = "okay";
	};
};

&sdio {
	max-frequency = <200000000>;
	no-sd;
	no-mmc;
	bus-width = <4>;
	cap-sd-highspeed;
	cap-sdio-irq;
	keep-power-in-suspend;
	mmc-pwrseq = <&sdio_pwrseq>;
	non-removable;
	sd-uhs-sdr104;
	pinctrl-names = "default";
	pinctrl-0 = <&sdmmc1m1_cmd &sdmmc1m1_clk &sdmmc1m1_bus4 &clk_32k>;
	status = "okay";
};

&pinctrl {

    sdio-pwrseq {
		wifi_enable_h: wifi-enable-h {
			rockchip,pins = <3 RK_PD2 RK_FUNC_GPIO &pcfg_pull_none>;
		};

        wifi_32k: wifi-32k {
			rockchip,pins = <0 RK_PA0 1 &pcfg_pull_none>;
		};
	};

    wireless-wlan {
		wifi_host_wake_irq: wifi-host-wake-irq {
			rockchip,pins = <3 RK_PD3 RK_FUNC_GPIO &pcfg_pull_down>;
		};
	};

    wireless-bluetooth {
		uart5_gpios: uart5-gpios {
			rockchip,pins =
				<1 RK_PD0 RK_FUNC_GPIO &pcfg_pull_none>;
		};
	};
};

&uart5 {
	status = "okay";
	pinctrl-names = "default";
	pinctrl-0 = <&uart5m1_xfer>;
};
```

编译时 bcmdhd 提示 dhd_mmc_suspend 未找到定义，所以就在 dhd_linux.c 添加 `volatile bool dhd_mmc_suspend = FALSE;`

同时把 Makefile 中的 CONFIG_DHD_USE_STATIC_BUF 取消定义就能编译通过了

bcmdhd 加载后使用 ifconfig wlan0 up 启动 wifi 芯片, 这时 bcmdhd 会去文件系统中找 wifi 芯片的 bin 文件固件和 txt 配置文件, 但是其路径配不好就写死在驱动中:

```c
void dhd_conf_set_path_params(dhd_pub_t *dhd, char *fw_path, char *nv_path)
{
    strcpy(fw_path, "/vendor/etc/firmware/fw_bcm4359c0_ag.bin");
    strcpy(nv_path, "/vendor/etc/firmware/nvram_ap6398s123.txt");
    // ....
};
```

然后将芯片固件复制到对应的目录下即可

##### 通过 wpa_supplicant 连接 wifi

```sh
vi /etc/wpa_supplicant.conf
# ctrl_interface=/var/run/wpa_supplicant
# ap_scan=1
# update_config=1
# network={
#     ssid="HUAWEI-1AAE9B"
#     psk="12345678"
#     key_mgmt=WPA-PSK
# }

wpa_supplicant -B -D nl80211 -i wlan0 -c /etc/etc/wpa_supplicant.conf

# 然后使用 udhcpc 来自动分配 ip
udhcpc -i wlan0
```


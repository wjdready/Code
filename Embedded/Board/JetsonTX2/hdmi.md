



## 使用hdmi

/home/shino/TX2/Linux_for_Tegra/source/kernel_src/hardware/nvidia/platform/t18x/quill/kernel-dts/tegra186-quill-p3310-1000-c03-00-base.dts

```c
	host1x {

		nvdisplay@15200000 {
				status = "okay";
				nvidia,dc-or-node = "/host1x/dsi";
		};
			dsi {
				status = "okay";
		 		nvidia,active-panel = <&panel_s_wuxga_8_0>;	
				panel-s-wuxga-8-0 {
							status = "disabled"; // 改为 disabled
						};
			};
		};
	sound {
		hdr40_snd_link_i2s: nvidia,dai-link-1 { };
	};
```


/home/shino/TX2/Linux_for_Tegra/source/kernel_src/hardware/nvidia/platform/t18x/quill/kernel-dts/tegra186-quill-p3310-1000-a00-00-base.dts

```c
	host1x {
		sor {
			status = "okay";
			dp-display {
				status = "disabled";
			};
			hdmi-display {
				status = "disabled";
			};

			panel-s-edp-uhdtv-15-6 {
				smartdimmer {
					status = "disabled";
				};
			};
		};

		dpaux@155c0000 {
			status = "disabled";
		};

		sor1 {
			status = "okay";
			nvidia,active-panel = <&sor1_hdmi_display>;
			hdmi-display {
				status = "okay";
			};
			dp-display {
				status = "disabled";
			};
		};

		nvdisplay@15200000 {
			status = "disabled";
		};

		nvdisplay@15220000 {
			status = "disabled";
		};
	};
```



## 使用 mipi 

/home/shino/TX2/Linux_for_Tegra/source/kernel_src/hardware/nvidia/platform/t18x/quill/kernel-dts/tegra186-quill-p3310-1000-c03-00-base.dts

```c
	host1x {

		nvdisplay@15200000 {
				status = "okay";
				nvidia,dc-or-node = "/host1x/dsi";
		};
			dsi {
				status = "okay";
		 		nvidia,active-panel = <&panel_s_wuxga_8_0>;	
				panel-s-wuxga-8-0 {
							status = "okay";
						};
			};
		};
	sound {
		hdr40_snd_link_i2s: nvidia,dai-link-1 { };
	};
```

/home/shino/TX2/Linux_for_Tegra/source/kernel_src/hardware/nvidia/platform/t18x/quill/kernel-dts/tegra186-quill-p3310-1000-a00-00-base.dts

```c
	host1x {
		sor {
			status = "disabled";
			dp-display {
				status = "disabled";
			};
			hdmi-display {
				status = "disabled";
			};

			panel-s-edp-uhdtv-15-6 {
				smartdimmer {
					status = "disabled";
				};
			};
		};

		dpaux@155c0000 {
			status = "disabled";
		};

		sor1 {
			status = "disabled";
			nvidia,active-panel = <&sor1_hdmi_display>;
			hdmi-display {
				status = "disabled";
			};
			dp-display {
				status = "disabled";
			};
		};

		nvdisplay@15220000 {
			status = "disabled";
		};
	};
```

## 编译和替换设备树

~/TX2/Linux_for_Tegra/source/kernel_src$ ./nvbuild.sh
ls /home/shino/TX2/Linux_for_Tegra/source/kernel_src/kernel/kernel-4.9/arch/arm64/boot/dts/tegra186-quill-p3310-1000-c03-00-base.dtb -lh

~/TX2/Linux_for_Tegra/AWA6280_System$ find ./ -name tegra186-quill-p3310-1000-c03-00-base.dtb

将上面设备树全部替换列出的项

/boot 目录下的 

/boot/tegra186-quill-p3310-1000-c03-00-base.dtb
/boot/dtb/kernel_tegra186-quill-p3310-1000-c03-00-base.dtb
/boot/kernel_tegra186-quill-p3310-1000-c03-00-base.dtb

试过替换了，没用。还是得重新完全烧录


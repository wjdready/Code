

翻译:
https://www.kernel.org/doc/Documentation/devicetree/bindings/sound/simple-card.txt


Simple-Card:

Simple-Card 描述了 SoC 和 codec 之间通过音频 DAI 的连接关系

必需的属性如下:

```js
compatible = "simple-audio-card";
```

可选属性如下:
```js
simple-audio-card,name = "my-sound"; // 用户指定的声卡名称，一个字符串属性
simple-audio-card,widgets = ""
```


## RV1106 声卡调试

设备树

```c
/ {
	acodec_sound: acodec-sound {
        compatible = "simple-audio-card";
        simple-audio-card,name = "rv-acodec";
        simple-audio-card,format = "i2s";
        // simple-audio-card,mclk-fs = <256>;
        simple-audio-card,bitclock-master = <&sound_master>;
        simple-audio-card,frame-master = <&sound_master>;

        simple-audio-card,cpu {
            sound-dai = <&i2s0_8ch>;
            system-clock-frequency = <12288000>;
        };
        sound_master: simple-audio-card,codec {
            sound-dai = <&ak5574>;
            system-clock-frequency = <12288000>;
        };
    };
};

&i2c1 {
    status = "okay";
    clock-frequency = <400000>;
    pinctrl-names = "default";
    pinctrl-0 = <&i2c1m1_xfer>;

    ak5574: ak5574@11 {
        #sound-dai-cells = <0>;
        reg = <0x11>;
        compatible = "asahi-kasei,ak5558";
        status = "okay";
    };
};

&i2s0_8ch {
	#sound-dai-cells = <0>;
    pinctrl-names = "default";
    pinctrl-0 = <&i2s0_pins>;
	status = "okay";
};

&acodec {
	status = "disable";
};
```

调试 i2c 可以使用 i2cdetect 和 i2cdump 来探测和读取寄存器信息, 然后当成功加载音频设备后就可以使用 arecord 来进行录音了

## UAC 调试

```sh
mount -t configfs none /sys/kernel/config
mkdir /sys/kernel/config/usb_gadget/rockchip -m 0770
echo 0x2207 > /sys/kernel/config/usb_gadget/rockchip/idVendor
echo 0x0019 > /sys/kernel/config/usb_gadget/rockchip/idProduct
echo 0x0100 > /sys/kernel/config/usb_gadget/rockchip/bcdDevice
mkdir /sys/kernel/config/usb_gadget/rockchip/strings/0x409 -m 0770
echo "0123456789ABCDEF" > /sys/kernel/config/usb_gadget/rockchip/strings/0x409/serialnumber
echo "rockchip" > /sys/kernel/config/usb_gadget/rockchip/strings/0x409/manufacturer

# 创建音频设备
echo "USB Audio Device" > /sys/kernel/config/usb_gadget/rockchip/strings/0x409/product
mkdir /sys/kernel/config/usb_gadget/rockchip/configs/b.1 -m 0770
mkdir /sys/kernel/config/usb_gadget/rockchip/configs/b.1/strings/0x409 -m 0770
echo 500 > /sys/kernel/config/usb_gadget/rockchip/configs/b.1/MaxPower
echo "uac1" > /sys/kernel/config/usb_gadget/rockchip/configs/b.1/strings/0x409/configuration
mkdir /sys/kernel/config/usb_gadget/rockchip/functions/uac1.gs0
ln -s /sys/kernel/config/usb_gadget/rockchip/functions/uac1.gs0 /sys/kernel/config/usb_gadget/rockchip/configs/b.1/uac1.gs0
echo ff400000.usb > /sys/kernel/config/usb_gadget/rockchip/UDC

ln -s 
```


测试音频:

```sh
rm /sys/kernel/config/usb_gadget/rockchip/configs/b.1/f1
ln -s /sys/kernel/config/usb_gadget/rockchip/functions/uac2.gs0 /sys/kernel/config/usb_gadget/rockchip/configs/b.1
ls /sys/class/udc > /sys/kernel/config/usb_gadget/rockchip/UDC
aplay -l 
aplay -D hw:1,0 test.wav

# 可以一边录音一遍播放
arecord -f S16_LE -r 48000 -c 2 -D hw:0,0 | aplay -D hw:1,0
cat /sys/kernel/debug/dmaengine/summary
```

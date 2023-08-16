

模组 esp8684-mini-1 下载程序

[模组官方文档](https://www.espressif.com/sites/default/files/documentation/esp8684-mini-1_mini-1u_datasheet_cn.pdf)

# [2023-08-16 11:24:20]

下载 AT [固件](https://docs.espressif.com/projects/esp-at/zh_CN/release-v3.0.0.0/esp32c2/AT_Binary_Lists/ESP32-C2_AT_binaries.html)

| 管脚  | 默认       | SPI 启动模式 | 下载启动模式 |
| ----- | ---------- | ------------ | ------------ |
| GPIO8 | 无         | 无关项       | 1            |
| GPIO9 | 内部弱上拉 | 1            | 0            |

# [2023-08-16 16:12:38]

esp8684-mini-1 模组，内置芯片 ESP8684，属于 ESP32-C2 系列. 

esp-at 主推的芯片系列，功能外设有限，功耗低，估计是专门用于模块化 wifi 或蓝牙的芯片系列.

参考[官网](https://www.espressif.com.cn/zh-hans/products/socs)给出的芯片系列.

32-bit RISC-V MCU & 2.4 GHz Wi-Fi & Bluetooth 5 (LE)

* RISC-V 32 位单核处理器，主频高达 120 MHz
* 行业领先的低功耗性能和射频性能
* 内置 272 KB SRAM（其中 16 KB 专用于 cache）、576 KB ROM 存储空间
* 14 个可编程 GPIO 管脚：SPI、UART、I2C、LED PWM 控制器、SAR 模/数转换器、温度传感器

坑一大堆，浪费老子一天时间! 以下是碰到的坑:

#### 坑1

首先发现官方的 esp8684-mini-1 模组没有出厂烧录有 AT 固件，串口一直输出报错, 无限重启

去哪下固件, --> 官方 github 上的 [esp-at](https://github.com/espressif/esp-at/), 选择 tags 可以看到最新的发布版本.

它的每个发布版本可能适合的芯片不同，要查看发布版本是否适用于该芯片。

#### 坑2

芯片内部引导程序在 UART0 默认输出波特率是 76800 而不是 115200，和其他系列芯片不同。

#### 坑3

硬件画的板子留出来通信的串口是UART0，UART0刚好可以用来下载固件, 然后回忆之前开发 ESP32-S 系列，下载需要在上电前将 GPIO0 和 GPIO2 引脚配置好。但是之前的经验并不能用于现在的板子，因为芯片不同的，用于 Boot 选择的管脚也可能改变了，这时应该去看[芯片手册](https://www.espressif.com.cn/sites/default/files/documentation/esp8684_datasheet_cn.pdf)。

[参考](https://blog.csdn.net/qq_62361151/article/details/130102202)

然后奇葩来了，按照要求设置，要进入下载模式 GPIO8 需要拉高，而 GPIO9 需要拉低，但是当时我错误的使用了 EN 口的 3.3V 给 GPIO8, 由于 EN 口接了一个 1uF 的电容，启动瞬间，1uF 延缓了 GPIO8 拉高的时间，导致启动后进入了未知模式, 这是根据串口提示的 `rst:0x1 (POWERON),boot:0x2 (ukw)` 信息发现的。

但是当时并不知道是电容导致的，于是我就怀疑是硬件虚焊导致，将模块重焊了2次还是没用。这时我才将疑点放在GPIO8 的3.3V 上，我当时怀疑是因为内部有电阻，和外部的上拉电阻形成了分压，导致启动时 GPIO8 没检测到高电平。

于是我将 GPIO8 直接接到 3.3V 处，这时再上电，终于进入下载模式，串口提示 `rst:0x1 (POWERON),boot:0x4 (DOWNLOAD(UART0))`。

不过这个怀疑是错的，能进入下载模式其实算是碰巧，因为接的那个 3.3V 那里没电容。也是因为后来才发现其实就是电容导致的，因为去掉 EN 的电容后是可以的。

#### 坑4

现在可以正常进入下载模式了，但是下载过程总是出错，我怀疑还是虚焊导致的，补焊问题依旧，又怀疑布线将串口线直接飞线到模块串口附近。第一次意外的成功了，后面问题依旧。

于是下午开始折腾直接使用模块，通过飞线的方式来下载。因为这样至少能排除是不是板子虚焊或布线的问题。但结果问题依旧。

这时我开始怀疑是 USB 转串口的问题，换了 ch340 的，结果每次下载都没有出错。我服了，那个 USB 转串口，支持 串口和 JTAG，估计是因为里面是软件转发的串口，软件估计有bug。导致大批量数据时出错。

看来以后最好选择存硬件实现的转串口比较保险。

#### 坑5

现在软件正常下载进去了，重启，发送一个 AT, 发现根本没有反应。每次串口显示都是 entry 0x403bb8a0 后就没反应了。

当时怀疑是 esp-at 的问题，是不是没有给到串口输出。这也导致后面花了大量时间去编译 esp-at 源码，主要是其中的下载环境真是有够慢。

等我好不容易编译出来固件，下载进去问题依旧。

就在不知如何解决的时候，偶然间在官方 [wiki](https://docs.espressif.com/projects/esp-at/zh_CN/release-v3.0.0.0/esp32c2/AT_Binary_Lists/index.html) 中看到，AT 通信使用的串口和下载调试使用的串口是不同的。

至此，我才终于能够在发 AT 后看到一个 OK.



# [2025-07-24 13:15:06]

EC20CE 使用高通 MDM9607

通用下载中心(包含驱动和固件): https://iot.quectelcn.com/download
通用下载专区(更多的下载): https://www.quectel.com.cn/download-zone

EC20CE资料, 登录后即可下载: https://www.quectel.com.cn/product/lte-ec20-ce


## 二次开发

源码: https://github.com/czplove/linux_kernel_ec20

在 `资料` 中下载 `硬件设计手册`, `QFlash` 下载工具

#### MDM9607 芯片资料


来自:
1. https://bbs.21ic.com/icview-2995192-1-1.html

#### 编译

输出 target 目录, 复制出来

#### 烧录

* 使用 9008 模式

9008 驱动: https://gitcode.com/Universal-Tool/8af66

根据硬件设计手册, 要进入 9008 模式, 需要将 USB_BOOT 设为高电平 1.8V, 所以短接 VDD_EXT 和 USB_BOOT 即可.

使用 QFlash 烧录


## 附录

相关资料:
1. 飞曙M2 Aurora随身WiFi_张大妈首测/拆: https://post.smzdm.com/p/a4p0mp08/
2. 高通芯片kernel开源(每个芯片单独分支): https://github.com/SoMainline/linux
3. 高通系列命名: https://zhuanlan.zhihu.com/p/578731761


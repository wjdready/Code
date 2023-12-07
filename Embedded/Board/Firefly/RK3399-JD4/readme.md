
# [2023-12-06 10:33:08]

官方 wiki: https://wiki.t-firefly.com/zh_CN/Core-3399-JD4/started.html

资料下载:
Core-3399-JD4 V1: https://www.t-firefly.com/doc/download/page/id/66.html
Core-3399-JD4 V2: https://www.t-firefly.com/doc/download/page/id/92.html

DriverAssitant 是 usb 驱动, RKDevTool (瑞芯微开发工具) 是烧录工具

> SocToolKit (瑞芯微工具套件) 是另一个烧录工具，适用于 RV1106 等 Soc

RKDevTool 支持多种烧录模式 (使用 USB3.0 口)

但最基本的还是 `Maskrom` 模式, 其他模式都在一定程度上需要事先运行一段加载程序。在核心板 EMMC 位置处有两个触点，上电时短接这两个触点进入 Maskrom 模式


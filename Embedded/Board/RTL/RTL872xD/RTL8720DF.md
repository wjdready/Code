
https://rtd-debug-zh.readthedocs.io/zh-cn/latest/index.html
https://aiot.realmcu.com/en/latest/rtos/index.html

板子链接
https://www.amebaiot.com/en/amebad/#rtk_amb25


AMB25

资料都没开放
https://rtd-debug-zh.readthedocs.io/zh-cn/latest/ambd_sdk/source/index.html

官方已经撤销 git 仓库, 原始仓库叫做 ambd_sdk, 搜索 github 可以获取一些用户当时的备份
https://github.com/ChandraMVE/ambd_sdk_MVE

## 2026-01-22 14:36:26

原本无法读取内存, 复位后可以读取什么原理

```sh
# JLink 
x/100x 0x08000000
# Cannot access memory at address 0x8000000
# gdb 发送 monitor reset 2
# Resetting target (2 = RESETPIN: Resets core & peripherals using RESET pin.)
```

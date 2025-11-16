

* 当连接出错时, 比如 undefined reference to `__imp_xxxx`
  查找 [Win32API](https://learn.microsoft.com/zh-cn/windows/win32/api/) 对xxxxx 进行搜索, 会找到其库名称, 然后在编译选项添加即可

* 开始 ssh 服务 net start sshd
  进不去? Permission denied 老火 

## MingW 的安装

虽然有 MSYS2 但是可能会推荐其他人更加小巧的 MingW, 所以还是有必要能够快速下载。

到[官网下载地址](https://www.mingw-w64.org/downloads/), Sources 部分有一个 [SourceForge 链接](https://sourceforge.net/projects/mingw-w64/files/mingw-w64/mingw-w64-release/), 最下面的编译好的

官网下载速度太慢，这个[地址给力](https://www.onlinedown.net/soft/10045442.htm), 注意下载连接是 `通用下载` 大小 100M 多


## 开启 ssh 服务器

net start sshd


## DSOC 文件安全管理系统

安装路径被刻意隐藏起来了，查看安装路径的方法:

任务管理器->服务->找到 DSOC 的其中一个服务, 例如 DGDlpService -> 右击打开服务 -> 再找到 DGDlpService 右击属性即可获知

已知安装位置为 `C:\Program Files (x86)\DSOC\TDLP` 包含 `TDLP` 和 `DSM` 等多个路径

目前尚不知如何在当前系统中删除，只能通过另一个系统进入磁盘进行删除。例如进入另一个系统，管理员打开 msys2 然后执行 `rm DSOC -rf`

## 下载 Windows 10 专业工作站版 22H2

magnet:?xt=urn:btih:6616eadd69cb418fdfbaf5f697de6320c714e25c&dn=zh-cn_windows_10_business_editions_version_22h2_updated_nov_2024_x64_dvd_c850f3d7.iso&xl=7008780288

激活 https://www.cnblogs.com/w205066376/p/18736604


## 通过网线共享网络

在网络共享中心，选择要共享的网卡，比如 `WLAN`，点击后选择`属性->共享`, 然后选择共享给某一个网卡，比如`以太网`

设置完后 `以太网` 会被默认设置成 192.168.137.1 子网掩码 255.255.255.0

在接收共享的主机上设置 IP 地址为 192.168.137.x 网段, 子网掩码 255.255.255.0，网关 192.168.137.1， DNS: 8.8.8.8

这些操作可以通过以下命令完成

```sh
# 设置 IP 地址、子网掩码、网关
netsh interface ip set address "以太网" static 192.168.137.100 255.255.255.0 192.168.137.1
# 设置 DNS
netsh interface ip set dns "以太网" static 8.8.8.8

# 接收主机是 Linux 设备嵌入式设备

# 设置 IP 地址和子网掩码
ifconfig usb0 192.168.137.100 netmask 255.255.255.0
# 设置网关 (关键)
route add default gw 192.168.137.1
# 设置 DNS
echo "nameserver 8.8.8.8" > /etc/resolv.conf
```

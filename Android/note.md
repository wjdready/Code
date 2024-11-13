
## 官方文档
https://source.android.google.cn/

root 之后装 termux 后支持下载大量工具，这时通过  adb shell 进入后导出 termux 的环境即可在 adb 中执行 termux 下载的命令:

```sh
export PATH=$PATH:/data/data/com.termux/files/usr/bin/
# export PATH=$PATH:/data/user/0/com.termux/files/usr/bin

# 创建一个目录并设置为 HOME，如此一来就可以用
mkdir /mnt/home && cd /mnt/home
export HOME=/mnt/home

# 可以弄一个 env.sh 将这两个环境自动导出，每次进入 adb, 只需 su, 然后
. /mnt/home/env.sh
```

远程登录 termux

```sh
pkg install openssh termux-auth
# 启动 ssh 并设置端口
sshd -p 2222
```

## 连接 usb 网卡

```sh
# 列出所有网卡信息
ip addr

# 给网卡分配固定 ip 地址
ifconfig eth1 192.168.8.15
```

---
mytags: myblog
title: Ubuntu 开发环境配置
date: 2025-03-13 13:09:14
categories: [Ubuntu, Linux]
tags: [开发环境]
---

本文主要介绍 ubuntu 开发环境的配置
<!-- more -->

#### 纯净版

```sh
# 安装 vmware tools
apt-get install open-vm-tools

# 开发必装 Ubuntu24Temp
apt-get install net-tools openssh-server chromium-browser vim

# ssh 免密登录 .ssh/authorized_keys

# 挂在共享文件夹
sudo mount -t fuse.vmhgfs-fuse .host:/ /mnt/hgfs -o allow_other
# 自动挂载 sudo vim /etc/fstab 中加入
# .host:/ /mnt/hgfs fuse.vmhgfs-fuse allow_other,defaults 0 0

```

#### Ubuntu24 开发环境版


```sh
# 在纯净版的基础上

# 基本工具
sudo apt-get install git curl

# 可编译 linux kernel 的配置,  并支持 arm64 交叉编译
sudo apt-get install make gcc flex bison libssl-dev gcc-aarch64-linux-gnu

# ============= flutter 开发 =============
sudo apt-get install clang cmake ninja-build  pkg-config libgtk-3-dev

# ============= TX2 开发 =============
sudo apt-get install qemu-user-static

# ============= 安卓开发 =============
sudo apt-get install openjdk-8-jdk g++-multilib gcc-multilib libxml2-utils m4
# 下载后移除 /etc/java-8-openjdk/security/java.security 文件里 jdk.tls.disabledAlgorithms 的 TLSv1, TLSv1.1
sudo ln -s /usr/lib/x86_64-linux-gnu/libncurses.so.6 /usr/lib/x86_64-linux-gnu/libncurses.so.5
sudo ln -s /usr/lib/x86_64-linux-gnu/libtinfo.so.6 /usr/lib/x86_64-linux-gnu/libtinfo.so.5
# 下载并编译源码安装 Python2.7
```

## 遇到的问题和解决

1. 以太网显示只有 10M/s

修改 .vmx 文件, 添加 ethernet0.virtualDev = "e1000" 


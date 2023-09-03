---
title: 使用 apt 仓库搭建交叉编译环境
date: 2023-09-03 10:00:48
categories: [交叉编译, apt]
tags: [交叉编译]
---

本文主要介绍如何直接基于 Ubuntu 使用 apt 在本机来搭建交叉编译环境

<!-- more -->

之前在开发 ns switch 的时候使用到 devkitpro，参考  `Embedded\Board\NSwitch\devpro.md`。

发现 devkitpro 提供了一个专门用来交叉编译的软件仓库，这个就很好。需要的包直接安装即可，不需要非常麻烦地从源码中构建了。

事实上 apt 本身也是支持同时安装多个平台地包的, 要安装指定平台的包，需在包名后指定 `package:arch`

```sh
sudo apt install qemu binfmt-support qemu-user-static
sudo dpkg --add-architecture arm64
sudo apt-get update

# 查看和移除
sudo dpkg --print-architecture
sudo dpkg --remove-architecture arm64

# 例如安装 arm64 的 vim 
sudo apt install vim:arm64
```

# [2023-09-03 10:12:28]

该方法并不成功, 尚且需要探究, 估计是 ubuntu 的问题, 可以换成 debain 系统试试





### Buildroot 入门

[官网](https://buildroot.org/)

按官方的介绍:

Buildroot 是一个简单、高效、易用的交叉编译生成嵌入式Linux系统的工具

Buildroot 结构简单，易于理解和扩展。它仅依赖于众所周知的 Makefile 语言

* 什么都能搞定: 交叉编译工具链、根文件系统生成、内核映像编译和引导加载程序编译
* 很容易: 由于其类似于内核的 menuconfig、gconfig 和 xconfig 配置界面，使用 Buildroot 构建基本系统很容易，通常需要 15-30 分钟
* 支持数千个包: X.org stack、Gtk3、Qt 5、GStreamer、Webkit、Kodi，支持大量网络相关和系统相关的实用程序

# [2023-10-23 16:27:46]

从克隆开始吧

```sh
git clone https://github.com/buildroot/buildroot.git

make defconfig
make -j8
```

目录结构

```sh
├── arch        # 
├── board
├── boot
├── configs
├── dl          # 下载的包放在这里
├── docs
├── fs
├── linux
├── output      
├── package
├── support
├── system
├── toolchain 
├── utils
├── Makefile
├── COPYING
├── DEVELOPERS
├── Config.in
└── README
```


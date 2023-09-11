---
mytags: myblog
title: QT 交叉编译
date: 2023-09-02 23:42:26
categories: [QT, 交叉编译]
tags: [QT]
---

本文主要介绍如何通过 QT 源码, 构建 QT 交叉编译库，并利用该库编译一个简单的 demo

<!-- more -->

构建脚本如下:

build.sh

```sh
#!/bin/bash

function download()
{
    wget -c http://mirrors.ustc.edu.cn/qtproject/official_releases/qt/5.15/5.15.2/single/qt-everywhere-src-5.15.2.tar.xz
    tar -xf qt-everywhere-src-5.15.2.tar.xz
}

function build()
{
    if [ ! -d "qt-everywhere-src-5.15.2" ]; then 
        download
    fi

    outdir=$PWD/out_qtbase
    
    cd qt-everywhere-src-5.15.2

    ./configure              \
        -prefix $outdir      \
        -xplatform linux-aarch64-gnu-g++ \
        -opensource          \
        -confirm-license     \
        -no-opengl           \
        -no-vulkan           \
        -no-compile-examples \
        -recheck-all

    # 这里只构建 qtbase, 可按需构建
    make module-qtbase -j8
    make module-qtbase-install_subtargets
}

build
```

首先 QT 源码编排是这样的:

在源码根目录有很多的文件夹, 每个文件夹对应于一个 qt 模块, 所有模块都需要依赖 qtbase 模块, 因为里面包含 qmake 构建工具, 同时qtbase 也是 QT 的核心, 里面包含 QT 的底层逻辑, UI 内核等。所以我们首先就是要构建 qtbase。

后续有需要再按需构建, 当然如果直接 make -j8 便是全部构建了, 会比较慢, 这里只构建一个 qtbase 写些简单程序足够了.

> 提示: make module- 后补全可以列出所有目标模块

#### 配置信息

下面是配置信息, 可通过 `./configure --help` 查看选项, 按需配置即可, 本文的配置内容如下, 其他没配置的就按默认。

```
-prefix 指明了编译后输出路径
-xplatform 指出交叉编译时的使用的目标 mkspec, mkspec 是 qt 内置的用于多平台的配置文件, 
    位于 qtbase/mkspecs 文件夹下, 每个 mkspec 是一个文件夹
    里面包含 qmake.conf, 描述编译时应当选择什么编译器以及配置,
    这里我们希望编译生成的库文件适用于 aarch64 的 linux 平台, 刚好对应于 linux-aarch64-gnu-g++，
    然后看里面的配置信息符合要求也不用改了。

值得注意的是 -xplatform 和 -prefix 配置信息将被硬编码至 qmake 程序里面的

-opensource
-confirm-license 必须要同意 QT 的开源协议
-no-opengl  
-no-vulkan  不使用 opengl 和 vulkan, 不知为了这个版本必须 -no-vulkan，否则编译在代码上就出错
-no-compile-examples 为了加快速度, 不编译示例程序, 但 make install 时, 
    示例代码是被一同复制到库目录的，所以后面想看哪个示例再单独进行编译即可
-recheck-all 每次执行都重新检查一下配置并测试, 也就说说丢弃所有之前缓存的配置测试的结果
```

#### 测试程序

编译完成后 qt 库将输出到 ./build.sh 所在目录下的 out_qtbase, 现在即可编写一个简单的 demo 测试一下

main.cpp

```cpp
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[]) 
{
    QApplication a(argc, argv);
    
    QLabel *label = new QLabel("Hello, world!");
    label->show();
    
    return a.exec();
}
```

demo.pro

```pro
QT += core gui widgets
TARGET = qtdemo
SOURCES += main.cpp
```

执行时使用生成的 qmake 来执行
```
../out_qtbase/bin/qmake
make
```

> 通过 `../out_qtbase/bin/qmake -query` 可以看到被硬编码到 qmake 中的路径信息和 mkspec 信息

#### 参考

* [1](https://zhuanlan.zhihu.com/p/573353841?utm_id=0)


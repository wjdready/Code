#!/bin/bash

# 导出 QMAKESPEC 变量可以更改 qmake 中硬编码的部分
export QMAKESPEC=/home/shino/code/Linux/source_debug/_rootfs/usr/lib/aarch64-linux-gnu/qt5/mkspecs/linux-aarch64-gnu-g++
export QT_SYSROOT=../_rootfs

qmake -set QT_SYSROOT ../_rootfs
qmake -query

qmake

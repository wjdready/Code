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

    # 这里只构建 qtbase
    make module-qtbase -j8
    make module-qtbase-install_subtargets
}

build


# [ref]
# https://zhuanlan.zhihu.com/p/573353841?utm_id=0

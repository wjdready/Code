---
mytags: myblog
title: 基于 Docker 搭建交叉编译环境
date: 2023-09-03 10:14:49
categories: [交叉编译]
tags: [交叉编译, Docker]
---

本文主要介绍如何使用 Docker 搭建交叉编译环境

<!-- more -->

关于 docker 交叉编译环境，已经有人做成[项目](https://github.com/dockcross/dockcross)开源了。我们只需使用即可


```sh
# 获取 linux-arm64 镜像
docker pull dockcross/linux-arm64

# 该镜像不能直接运行, 需要间接运行，我是真的服了
# 根据提示, 将运行报错导出成一个脚本，使用该脚本来运行

docker run --rm dockcross/linux-arm64 > dockcross-linux-arm64
chmod +x dockcross-linux-arm64

# 导出的脚本是 bash 这意味着 windows 直接不能用了啊, 可能默认觉得 Windows 用户一定使用 WSL 吧
# 不过我是在 Windows 的 MSYS2 下, 有 bash 环境，但还是报错, 后排查是路径名问题
# 因此更改生成的 dockcross-linux-arm64 脚本
HOST_PWD=$(echo $HOST_PWD | sed 's/\//\\\//g')

# 最后通过这个脚本执行 bash, 这个执行一次就退出
# 注意: -c 是 bash 的参数, 不是 ./dockcross-linux-arm64
./dockcross-linux-arm64 bash -c 'echo Hello World'

# 可直接处理本地文件, 例如编译本机的 hello.c
# 其原理也很简单, 就是 docker 启动时通过 -v 当前路径到容器里面即可
./dockcross-linux-arm64 bash -c '$CC hello.c -o hello'
```

搞了半天发现不支持直接安装 arm64 的包, 麻烦的地方不在于交叉编译一个简单的hllo world 而是编译需要依赖各种 qt, opencv, ssl 等库，手动编译这些库比较麻烦。

而这个 docker 环境也只是简单地安装了一下交叉编译器，既然如此那我为何不直接使用 vm 虚拟机或 WSL 里交叉编译。

#### 基于 Debain 构建

既然 dockcross 不好用, 那就自己创建一个 docker 镜像好了

参考 i96751414/cross-compiler-linux-arm64, 发现这个也是基于 Debain 容器镜像

Dockerfile 如下

```dockerfile
from debian

# 添加 arm64 仓库
RUN dpkg --add-architecture arm64
RUN apt-get update

# 本机工具
RUN apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu make git file -y

# arm64 端库
RUN apt install qtbase5-dev:arm64 apt install libqt5svg5-dev:arm64 -y

# 替换 qmake
RUN rm /usr/bin/qmake
RUN ln -s /usr/lib/aarch64-linux-gnu/qt5/bin/qmake /usr/bin/qmake
```

执行:

```sh
# -t,--tag: 指定镜像名称
docker build -t aarch64 .

# 构建镜像完成后查看所有镜像, 找到刚才创建的 ID 3d0ee59732ed
docker images

# 运行镜像, 并挂载自己的工程路径到 docker 容器中, -i,--interactive 保持打开, -t,--tty 分配一个伪终端
docker run -v /c/Users/shino/Desktop/qdraw:/mnt -it aarch64 bash

# 可在里面进行 qmake && make 操作, 当然一切顺利后续可以直接执行如下命令。(--rm: 运行后立即删除容器)
docker run -v /c/Users/shino/Desktop/qdraw:/mnt --rm -it aarch64 bash -c "cd /mnt && qmake && make -j8"
```

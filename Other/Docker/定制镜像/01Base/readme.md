#### 基于 Debain 构建交叉编译环境

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


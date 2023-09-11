from debian

# 添加 arm64 仓库
RUN dpkg --add-architecture arm64
RUN apt-get update

# 本机工具
RUN apt install gcc-aarch64-linux-gnu g++-aarch64-linux-gnu make git file -y

# arm64 端库
RUN apt install qtbase5-dev:arm64 libqt5svg5-dev:arm64 -y

# 替换 qmake
RUN rm /usr/bin/qmake
RUN ln -s /usr/lib/aarch64-linux-gnu/qt5/bin/qmake /usr/bin/qmake

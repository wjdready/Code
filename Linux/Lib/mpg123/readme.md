


[官网源码](https://www.mpg123.de/download.shtml)

```sh
TOOLS_PREFIX=/home/shino/code/Board/Luckfox/luckfox-pico/sysdrv/source/buildroot/buildroot-2023.02.6/output/host/bin/arm-rockchip830-linux-uclibcgnueabihf

# 设置交叉编译工具链
export CC=$TOOLS_PREFIX-gcc
export CXX=$TOOLS_PREFIX-g++
export AR=$TOOLS_PREFIX-ar
export RANLIB=$TOOLS_PREFIX-ranlib
export LD=$TOOLS_PREFIX-ld

# 运行配置
./configure --host=arm-linux-gnueabi --prefix=$PWD/out --with-audio=alsa

# 编译和安装
make
make install
```

使用:

```sh
# 播放网络电台:
# http://lhttp.qingting.fm/live/4518/64k.mp3    // 浙江之声
# 到 https://www.qtfm.cn/radiopage 打开对应的电台，记下地址对应的数字替换上面的数字即可
# 网易云音乐 http://music.163.com/song/media/outer/url?id=37653546.mp3
# 到 https://music.163.com 点击对于音乐得到 id, 然后进行播放. 仅非 VIP 歌曲有效
mpg123 http://lhttp.qingting.fm/live/4518/64k.mp3
```


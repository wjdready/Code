

# [2023-03-15 13:53:42]

K210 似乎是很强大的板子，有机会尝试看看开发起来怎么样。

[官方 sdk](https://github.com/kendryte/kendryte-standalone-sdk.git)

[官方工具链](https://github.com/kendryte/kendryte-gnu-toolchain/releases/download/v8.2.0-20190409/kendryte-toolchain-win-i386-8.2.0-20190409.tar.xz)

[开发板官方](https://www.yahboom.com/study/K210-Developer-Kit), 这时后来才找到的.

编译 helloworld, 本来想使用 MSYS 自带的 RISC-V GCC 工具链, 但是奈何官方官方执意要求使用 mingw32-make 来编译, 服了

```
grep -nr "mingw32-make" ./
./kendryte-standalone-sdk/cmake/toolchain.cmake:33:        global_set(CMAKE_MAKE_PROGRAM "${TOOLCHAIN}/mingw32-make${EXT}")
```

### 编译

```sh
# 下载源码和工具
git clone https://github.com/kendryte/kendryte-standalone-sdk.git
wget https://github.com/kendryte/kendryte-gnu-toolchain/releases/download/v8.2.0-20190409/kendryte-toolchain-win-i386-8.2.0-20190409.tar.xz
tar -xf kendryte-toolchain-win-i386-8.2.0-20190409.tar.xz

# 编译 hello_world
cd kendryte-standalone-sdk
mkdir build -p && cd build
export KTOOLS_PATH=/c/Users/shino/Desktop/CodeNote/Embedded/Board/K210/YB-DKA01/_sdk/kendryte-toolchain/bin
cmake .. -DPROJ=hello_world -DTOOLCHAIN=$KTOOLS_PATH -G "MSYS Makefiles"
```

OK 生成两个文件 hello_world 和 hello_world.bin, hello_world 是 elf 格式的，可用于调试以及使用 JLink 下载，hello_world.bin 使用官方下载工具下载。

### 下载

官方下载工具中提供两种，一个是 C# 写的 GUI 下载工具 [K-Flash.zip](https://github.com/kendryte/kendryte-flash-windows/releases)，另一个是 [Python 脚本工具](https://github.com/kendryte/kflash.py)

```sh
git clone https://github.com/kendryte/kflash.py.git
cd kflash.py

# 安装到 Python 环境中, 后面就可以在任意目录执行 kflash 进行下载
./setup.py install

# 下载到板子
kflash hello_world
```

# [2023-03-15 14:35:54

这个官方 SDK 也太精简了吧，什么都没有，就只是一个给出一个 hello_world 的运行示例，driver 有一些常见的外设，这些外设好像也足够了，似乎没有 SDIO, SD 卡可能是用 SPI 驱动的。

后来发现原来官方把 demo 和 sdk 分开了, 官方的[kendryte-standalone-demo](ttps://github.com/kendryte/kendryte-standalone-demo.git) 仓库，还是挺多例子的。

后面发现, 已经有人移植 RT-Thread 到 K210了, [仓库链接](https://github.com/RT-Thread/RT-AK-plugin-k210)

[Sipeed](https://dl.sipeed.com/shareURL/MAIX/HDK/Sipeed-Maix-Bit) 的资料还是挺全的

但是芯片手册是真难找啊, 找不到.
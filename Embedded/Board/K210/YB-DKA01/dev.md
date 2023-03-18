

# [2023-03-15 13:53:42]

K210 似乎是很强大的板子，有机会尝试看看开发起来怎么样。

[官方 sdk](https://github.com/kendryte/kendryte-standalone-sdk.git)

[官方工具链](https://github.com/kendryte/kendryte-gnu-toolchain/releases/download/v8.2.0-20190409/kendryte-toolchain-win-i386-8.2.0-20190409.tar.xz)

[开发板官方](https://www.yahboom.com/study/K210-Developer-Kit), 这是后来才找到的.

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


# [2023-03-16 11:22:00]

为啥编译一个 hello_world 这么大，有 1MB 多, 怀疑是 cmake 中设置了链接到了某个库，首先看顶层 Cmake 脚本, 前面只是加载源文件之类的，关键地方是 

```cmake
include(./cmake/executable.cmake)
```

可见 executable.cmake 是用于生成程序的脚本, 在该文件可以看到 

```cmake
target_link_libraries(${PROJECT_NAME} kendryte...)
```

也就说说链接到了 libkendryte.a，在 build 中确实看到 libkendryte.a 有 5MB 多, 而且这里面包含 bsp 和 driver 不能取消链接该 libkendryte.a, 只能对生成该 libkendryte.a 的链接库中进行精简。在源码的 lib 目录中的 CMakeLists.txt 中看到

```cmake
TARGET_LINK_LIBRARIES(kendryte PUBLIC nncase-wrapper)
```

而 nncase-wrapper 又链接自:

```cmake
target_link_libraries(nncase-wrapper PRIVATE nncase-v0 nncase-v1)
```

最终的链接文件是一个给定的二进制 .a 文件: libnncase.runtime.a 这个文件 12MB, 这才是导致 hello_world 中程序很大的原因。`nncase` 是用于深度学习的，如果只是编译测试一些外设功能，可以不进行链接，因此在 `TARGET_LINK_LIBRARIES(kendryte PUBLIC nncase-wrapper)` 中直接禁止好了。即:

```
TARGET_LINK_LIBRARIES(kendryte)
```

如此一来，编译 hello_world 就只有 92KB 了。


# [2023-03-16 11:48:02]

查看 kflash.py 的源码, 发现官方的  ISP bootloader 是直接集成在 Python 里面的, 它是一个压缩后的 hex 文件

```python
ISP_PROG = '789cbcbc7d5c1357d6007c6792c924080a0...' # 64KB
ISP_PROG = binascii.unhexlify(ISP_PROG)
ISP_PROG = zlib.decompress(ISP_PROG)
```

kflash.py 支持指定 bootloader 文件, 如果命令行参数没有给定 bootloader 文件则使用本地变量

```python
# install bootloader at 0x80000000
isp_loader = open(args.bootloader, 'rb').read() if args.bootloader else ISP_PROG
self.loader.install_flash_bootloader(isp_loader)
```

下载原理是在通过 UART 让 CPU 进入 ISP 模式，然后通过 ISP 下载 bootloader 到内存，bootloader 再和串口进行通信从而下载固件到想下载的位置。这样做的好处是 Boot 是完全可以自定义的，下载位置可根据 Boot 进行改变。

好像官方并没有开源 bootloader，在一些个人的 github 中找到了个人实现的 [bootloader](https://github.com/loboris/Kboot.git) 

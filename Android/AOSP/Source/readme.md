


[AOSP清华镜像站](https://mirrors.tuna.tsinghua.edu.cn/help/AOSP/)

# [2023-11-21 20:44:21]

内存太小导致编译失败，这里通过删除原来的 swap 然后创建更大的 swap 来解决

```sh
# 首先关闭并删除现存的 swap
sudo swapoff /swapfile
sudo rm /swapfile

# 创建新 swap
sudo dd if=/dev/zero of=/swapfile bs=1G count=32
sudo chmod 0600 /swapfile
sudo mkswap -f /swapfile

# 使用新 swap
sudo swapon /swapfile
```

目录结构

```sh
du -d 1 -h | sort -h

408K    ./libnativehelper
896K    ./pdk
1.3M    ./kernel
7.5M    ./platform_testing
16M     ./bootable
16M     ./build            # 存放构建脚本和框架
27M     ./dalvik
30M     ./sdk
44M     ./bionic
84M     ./art
86M     ./libcore
102M    ./toolchain
149M    ./development
191M    ./hardware
402M    ./test
441M    ./developers
505M    ./system
842M    ./packages
1.4G    ./cts
1.7G    ./tools             # 用于开发和构建 Android 系统的工具和实用程序
1.8G    ./device            # 用于支持特定设备的代码目录
1.9G    ./frameworks        # 包含 Android 系统的各种框架和库
7.8G    ./external          
21G     ./out               # 构建时输出目录
33G     ./prebuilts         # 提供预先构建的二进制工具，支持 linux 和 mac 平台
```

## 构建系统

[官方文档](https://source.android.google.cn/docs/setup/build?hl=fi)

最新的安卓构建系统是由 Google 开发的 Soong，，用于替代之前的Make构建系统。

Soong 采用了 Go 语言作为实现语言，它的运行机制可以简单地理解为以下几个步骤：

1. Blueprint 文件定义：Soong 使用一种名为 Blueprint 的声明性语言来定义构建规则。Blueprint 文件描述了构建过程中所需的模块、依赖关系和编译选项等信息
2. 解析和扩展：Soong解析Blueprint文件并根据其中的规则生成构建图。构建图表示了模块之间的依赖关系和构建顺序
3. 生成Build.ninja文件：Soong 将构建图转换为 Build.ninja 文件，该文件是 Ninja 构建系统的输入文件。Build.ninja文件描述了每个模块的编译命令和依赖关系
4. Ninja构建：使用生成的Build.ninja文件，Soong调用Ninja构建系统来执行实际的编译过程。Ninja会根据依赖关系和编译命令来并行地构建各个模块


# [2023-11-29 14:40:06]

各项模块都输出到了 `out/soong/.intermediates`


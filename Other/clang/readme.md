

## 交叉编译

```sh
# 查看支持的架构
llc --version
clang++ -print-targets

########## 1. 创建简单的 sysroot #############

# --verbose 可查看 clang++ 编译时完整参数, 对排查报错原因有帮助
# --target 指定目标架构, arm 为 arm-linux-gnueabihf
# -fuse-ld 指定连接器, 可以指定 clang 自带的 ldd: -fuse-ld=lld
clang++ main.cpp  \
    --target=aarch64-linux-gnu \
    --sysroot C:/Users/shino/Desktop/clang/libc \
    -fuse-ld=C:/ProgramFiles/Library/aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-ld

# 这里 sysroot 在交叉编译时需要指定，代表运行时的环境, 其实相当于 linux 的根目录，一般来说在本机开发，默认就是 /
# 但是交叉编译的话，需要构建一个 sysroot 的环境，例如使用 ubuuntu-base 构建的环境, 里面需要至少包含有 cpp 开发的环境
# 其实每个 gcc 交叉工具链都自带一个 sysroot 环境了，如果只使用 cpp 标准库，其他库自行通过源码解决的话，使用 交叉工具链自带的 sysroot 即可
# 下面介绍 windows 下基于 C:/ProgramFiles/Library/aarch64-none-linux-gnu 的 sysroot 构建
# 首先，打印 sysroot 的路径
C:/ProgramFiles/Library/aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc --print-sysroot 
# 显示出来的位置就是 sysroot 的路径，单独复制出来，因为一会还要往里面添加内容，例如复制到 C:/Users/shino/Desktop/clang/libc
# 接下来复制 gcc 库到 sysroot 下, 因为 clang 编译时用到 gcc 库的 libgcc.a, crtbeginS.o 等
cp -r C:/ProgramFiles/Library/aarch64-none-linux-gnu/lib/gcc C:/Users/shino/Desktop/clang/libc/usr/lib64/
# 然后还要把 c++ 的头文件复制到 sysroot 下
cp -r c:/ProgramFiles/Library/aarch64-none-linux-gnu/aarch64-none-linux-gnu/include/c++ C:/Users/shino/Desktop/clang/libc/usr/include/
# 最后指定连接器 -fuse-ld 为 aarch64-none-linux-gnu-ld 就可以了, 当然也可以使用 clang 自带的 ldd 更简单了, 推荐使用，这里只是演示完全标准库和链接完全使用 gcc
# 在这种情况下，clang 只是起到编译的作用，链接目标平台时使用 gcc 的链接器，如果不设置 -fuse-ld, 默认是 ld, 如果环境变量路径中没有 ld, 会出现找不到程序
# clang++: error: unable to execute command: program not executable

########## 2. 使用 libcxx 构建动态库 #############
# 使用 gcc 编译出来的动态库在链接时，依赖对应版本的 gcc 库，工具链自带的 gcc 运行库版本太低就会出现找不到 GLIBC_VERSION 错误
# 这时可以使用 clang 的 libcxx 并静态编译, 使得动态库不依赖目标平台的运行库 (当然使用不依赖目标平台动态库，编译成可执行程序时还是需要依赖目标平台的 glibc 运行库的)
# 首先获取 clang 目标平台的 libcxx, 现在不知道怎么编译生成 arm 平台的 libcxx，只能从 flutter/engine/src/flutter/buildtools/linux-x64/ 中获取, 可以看到
# 在该目录下的 clang/lib/aarch64-unknown-linux-gnu 有我们需要的 libc++.a  libc++experimental.a  libunwind.a
# 复制到自己安装的 clang 目录下,
cp -r clang/lib/aarch64-unknown-linux-gnu C:/ProgramFiles/Library/clangllvm/lib
# 当然还有头文件
cp -r clang/include/aarch64-unknown-linux-gnu C:/ProgramFiles/Library/clangllvm/include
# 当使用 -stdlib=libc++ 时, clang 就会根据 target 去这些目录中查找对应的 libc++.a 

clang++ main.cpp \
    --target=aarch64-linux-gnu \
    --sysroot C:/Users/shino/Desktop/clang/libc \
    -stdlib=libc++ \
    -fuse-ld=lld -shared -fPIC \
    -o libshare_example.so

# 这时 libshare_example.so 就可以直接使用了，不用依赖目标平台的运行库
aarch64-none-linux-gnu-g++ test.cpp -L./ -lshare_example -o example
```

main.cpp

```cpp
#include <iostream>
#include <cmath>

float addsin(int a, int b)
{
    return sinf(a + b);
}

int main()
{
    std:: cout << "addsin(1, 12) = " << addsin(1, 12) << std::endl;
    return 0;
}
```

test.cpp

```cpp
#include <stdio.h>

extern float addsin(int a, int b);

int main(int argc, char const *argv[])
{
    float s = addsin(1, 12);

    printf("s = %.2f", s);

    return 0;
}
```


参考文档
https://llvm.org/docs/CMake.html#quick-start
https://libc.llvm.org/full_host_build.html
https://llvm.org/docs/HowToCrossCompileLLVM.html

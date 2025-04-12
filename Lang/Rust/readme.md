
官网:
https://www.rust-lang.org/

官方参考手册
https://doc.rust-lang.org/reference/macros.html

官方教程:
https://doc.rust-lang.org/book/title-page.html

官方通过例子来学习(中文):
https://rustwiki.org/zh-CN/rust-by-example/custom_types/structs.html

## rust 安装

参考[国内安装方法](https://blog.csdn.net/weixin_44242403/article/details/142363803)

```sh
# 1. 下载 rustup-init
# https://mirrors.aliyun.com/rustup/rustup/dist/x86_64-pc-windows-msvc/rustup-init.exe

# 2. 在终端中运行， 为了方便增加环境变量
set RUSTUP_DIST_SERVER=https://mirrors.ustc.edu.cn/rust-static
set RUSTUP_UPDATE_ROOT=https://mirrors.ustc.edu.cn/rust-static/rustup

# 3. 然后再运行 rustup-init
# 完成会自动创建 C:\Users\shino\.cargo 和 C:\Users\shino\.rustup 文件夹
# 只需将 C:\Users\shino\.cargo\bin 导出环境即可

# 4. 配置 cargo 用于下载依赖包时加速
# 当然也可以直接将上面两个环境变量 RUSTUP_DIST_SERVER 和 RUSTUP_UPDATE_ROOT 默认导出即可
# code C:\Users\shino\.cargo\config
[source.crates-io]
replace-with = 'ustc'

[source.ustc]
registry = "sparse+https://mirrors.ustc.edu.cn/crates.io-index/"
```


```sh
# msvc 和 gnu 工具链切换
rustup default stable-gnu
rustup default stable-msvc
```

#### Windows 32 位支持

```sh
set RUSTUP_UPDATE_ROOT=https://mirrors.tuna.tsinghua.edu.cn/rustup/rustup
set RUSTUP_DIST_SERVER=https://mirrors.tuna.tsinghua.edu.cn/rustup

rustup toolchain install stable-i686-pc-windows-msvc
rustup target add i686-pc-windows-msvc
rustup default stable-i686-pc-windows-msvc

# 切换回来
rustup default stable-x86_64-pc-windows-msvc
```

#### 设置 rust 仓库镜像提高下载速度

在 C:/Users/xxx/.cargo/config 中添加

```
[source.crates-io]
replace-with = 'ustc'

[source.ustc]
registry = "https://mirrors.ustc.edu.cn/crates.io-index"
```

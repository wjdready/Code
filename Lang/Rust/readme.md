
官网:
https://www.rust-lang.org/

官方参考手册
https://doc.rust-lang.org/reference/macros.html

官方教程:
https://doc.rust-lang.org/book/title-page.html

官方通过例子来学习(中文):
https://rustwiki.org/zh-CN/rust-by-example/custom_types/structs.html


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

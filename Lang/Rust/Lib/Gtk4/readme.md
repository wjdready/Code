
入门:
https://gtk-rs.org/gtk4-rs/stable/latest/book/installation_windows.html


## msvc 的构建

```sh
pip install gvsbuild

# 构建依赖 msys2 中的一些命令, 生成的 msvc 不依赖
choco install msys2
gvsbuild build gtk4

# 构建完成后导出生成的目录
set PATH=%PATH%;C:\gtk-build\gtk\x64\release\bin
set PKG_CONFIG_PATH=C:\gtk-build\gtk\x64\release\lib\pkgconfig

# cargo 切换到 msvc 目标, 注意在 cmd 下构建, msys2 环境容易链接的是 gnu 库
rustup default stable-msvc
```

还需添加 link 库路径, `build.rs` 如下

```rust
fn main() {
    println!("cargo:rustc-link-search=native=C:/gtk-build/gtk/x64/release/lib");
}
```

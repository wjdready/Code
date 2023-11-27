


```sh
npm create tauri-app
cnpm install
npm run tauri build --release

# 获取 tauri cli 可直接编译
# MSYS2 可直接下载 cargo-tauri 来提供 tauri 命令
cargo-tauri build --target=i686-pc-windows-msvc

# 对于 Ubuntu 构建需要以下依赖
sudo apt-get install rust-all libwebkit2gtk-4.0-dev librsvg2-dev
```

参考
https://www.psvmc.cn/article/2023-02-10-tauri-start-01.html

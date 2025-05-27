

```sh
# 列出所有目标平台
rustup target list

# 安装目标平台工具链
rustup target add thumbv7m-none-eabi

# 构建目标平台
cargo build --target thumbv7m-none-eabi
```

使用 arm-none-eabi-gdb 调试

Rust 进行嵌入式开发:
https://xxchang.github.io/book/intro/index.html

```sh
# 列出所有目标平台
rustup target list

# 安装目标平台工具链
rustup target add thumbv7m-none-eabi

# 创建工程
cargo new demo && cd demo
cargo add cortex-m-rt
cargo add panic-halt
cargo add cortex-m-semihosting
```

`main.rs`

```rust
#![no_main]
#![no_std]

use panic_halt as _;

use cortex_m_rt::entry;
use cortex_m_semihosting::{debug, hprintln};

#[entry]
fn main() -> ! {
    hprintln!("Hello, world!");

    // 退出 QEMU
    debug::exit(debug::EXIT_SUCCESS);

    loop {}
}
```

`.cargo/config.toml`


```ini
[build]
# target = "thumbv6m-none-eabi"    # Cortex-M0 and Cortex-M0+
target = "thumbv7m-none-eabi"      # Cortex-M3
# target = "thumbv7em-none-eabi"   # Cortex-M4 and Cortex-M7 (no FPU)
# target = "thumbv7em-none-eabihf" # Cortex-M4F and Cortex-M7F (with FPU)

[target.thumbv7m-none-eabi]
# 当执行 cargo run 时执行
runner = "qemu-system-arm -cpu cortex-m3 -M lm3s6965evb -nographic -semihosting-config enable=on,target=native -kernel"
rustflags = [
    "-C", "link-arg=-Tlink.x",
]
```

link.x 由 cortex-m-rt 提供，不需要创建和设置, 这时只需设置 MEMORY 即可

`memory.x`

```php
MEMORY
{
    FLASH : ORIGIN = 0x00000000, LENGTH = 256K
    RAM : ORIGIN = 0x20000000, LENGTH = 40K
}
```

> qemu 中似乎无法使用 FLASH ? 0x00000000 应该是一个内存区域, 模拟 flash
> 如果硬写 stm32 的 flash 0x08000000 实际地址, qemu 运行会报错
> 当要调试时可以添加 `-S -s` 到 Qemu，然后使用 VSCode + gdb 对生成的 elf 进行调试即可

## 实机测试

将 memory.x 中的地址改回 flash 实际地址 0x08000000, 然后通过 openocd 下载和调试

`openocd.cfg`

```sh
source [find interface/stlink.cfg]
source [find target/stm32h7x.cfg]

reset_config none separate

# 下载程序
program ./target/thumbv7m-none-eabi/debug/demo verify

# 启动半主机模式
arm semihosting enable
```

#![no_std]
#![no_main]

use core::{panic::PanicInfo};
use cortex_m_semihosting::{debug, hprintln};

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

// 使用 no_mangle 属性避免符号修饰，extern "C" 调用约定
#[unsafe(no_mangle)]
fn main() -> ! {
    // 使用半主机模式打印信息
    hprintln!("Hello, semihosting!");

    // 模拟一些工作
    for i in 0..5 {
        hprintln!("Counter: {}", i);
    }

    // 退出调试会话
    debug::exit(debug::EXIT_SUCCESS);
    loop {}
}

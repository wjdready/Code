use std::process::Command;
use std::env;

fn main() {
    let manifest_dir = env::var("CARGO_MANIFEST_DIR").unwrap();
    let start_s_path = format!("{}/src/start.s", manifest_dir);
    let start_o_path = format!("{}/src/start.o", manifest_dir);

    // 使用 llvm-mc 编译 start.s 为 start.o
    let status = Command::new("llvm-mc")
       .args(&[
            "-triple=thumbv7m-none-eabi",
            "-filetype=obj",
            "-g", // 添加调试信息
            "-show-encoding", // 显示指令编码信息
            "-o", &start_o_path,
            &start_s_path,
        ])
       .status()
       .expect("Failed to execute llvm-mc");

    if !status.success() {
        panic!("llvm-mc failed to compile start.s");
    }

    // 打印链接脚本路径
    println!("cargo:rustc-link-arg=-Tlink.ld");
    // 打印不使用标准启动文件的参数
    println!("cargo:rustc-link-arg={}", start_o_path);
    // 打印链接搜索路径
    println!("cargo:rustc-link-search={}", manifest_dir);
}

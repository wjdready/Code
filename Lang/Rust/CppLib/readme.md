
C Rust 相互调用:

## 直接使用 rustc 编译

```rs
#[link(name = "add", kind = "static")]
extern "C" {
    fn add(a: i32, b: i32) -> i32;
}

fn main() {
    println!("Hello, world! {}", unsafe { add(1, 2) });
}
```

```sh
ls 
# add.lib main.rs
rustc main.rs
```

可用选项
```sh
# 如果编译时指定 -ladd 就不需要源文件中定义 #[link]
# --target 可指定生成目标平台, 执行 rustc --print target-list 查看所有支持的目标平台
# -C <option> 可指定编译器选项, 例如这里设置了链接参数
-ladd --target i686-pc-windows-msvc -C link-args="User32.lib"
```

## 使用 cargo

```sh
cargo new demo
# 将 add.lib 放在 lib
ls
# Cargo.toml  lib  src  target
```

可在 rs 中直接定义 `#[link(name = "lib/add", kind = "static")]` 即可直接编译运行

## 指定 target

cargo 命令行中

```makefile
all:
	RUSTFLAGS="-Clink-arg=User32.lib" \
	cargo build	--release	\
	--target i686-pc-windows-msvc
```

还可以自定义 build 过程

build.rs
```rs
fn main() {
    println!("cargo:rustc-link-lib=static=add");
    println!("cargo:rustc-link-search=native=lib");
}
```

也可以定义在 `.cargo/config.toml` 中:

```ini
[target.i686-pc-windows-msvc]
rustflags = [
    "-C", "link-lib=static=add",
    "-C", "link-search=native=lib",
    "-C", "-Clink-arg=User32.lib"
]
```

该文件会在编译阶段被编译并执行，然后输出到中间文件 output，该文件在编译时被读取，从而对编译过程进行配置

更多信息和编译选项[参考](https://doc.rust-lang.org/cargo/reference/build-scripts.html)


##### 参考文献
https://doc.rust-lang.org/nomicon/ffi.html
https://www.cnblogs.com/mxnote/articles/16779876.html
https://zhuanlan.zhihu.com/p/659665127


## Hello World

```rs
fn main() {
    println!("Hello, world!");
}
```

> ! 是表示 println! 是一个宏，而非函数
> 需要编译 rustc mian.c 

## Cargo 

值得说明的是 VSCode 插件 rust-analyzer 需要是 cargo 项目才会有提示，单独的 rs 文件似乎不生效

```sh
# 使用 Cargo 创建项目, 当不在 git 仓库内, cargo 会自动初始化一个 git 仓库
cargo new hello

# 构建项目, 默认为debug, 发布用 --release, 将被输出到 target
cargo build

# 通过 cargo 运行, run 时是可以自动检测文件被修改，然后会自动执行 build
cargo run

# check 比 build 快得多, 可以快速检测代码可以编译，但是不会生成可执行文件
cargo check

```

下面将从一个 Demo 出发以对 rust 有一个整体的了解

## 猜谜游戏
它的工作原理如下：程序将生成一个 1 到 100 之间的随机整数。然后它会提示玩家输入猜测值。输入猜测后，程序将指示猜测是否太低或太高。如果猜测正确，游戏将打印一条祝贺消息并退出。

```rs
// 关键字  use  用于将模块导入当前作用域
// 这里使用标准库 io 库, 该语句将 std::io 引入作用域
// 否则, 需要使用完整的引用 std::io::stdin()
use std::io;

fn main() {
    println!("Guess the number!");

    println!("Please input your guess.");

    // 变量默认是不可变的, 对于可变的变量必须使用 mut 声明
    // String 是标准库提供的字符串类型
    // new 函数是各种类型的通用函数用于从堆中构造对象
    let mut guess = String::new();

    // stdin 函数将返回一个 std::io::Stdin 实例，它是一个类型，表示终端的标准输入的句柄
    // read_line() 是 Stdin 类型中的一个方法以获取用户的输入
    // read_line() 将以附加的形式追加到输入的参数
    // read_line() 返回一个 Result 实例， Result 是一个枚举(enum)，是一种可以处于多种可能状态之一的类型
    // Result 类型有一个可以调用的 expect 方法, 如果是变体 Err, 则将异常出错，
    // 如果是变体 Ok 则返回获取的返回值，即输入字符串
    io::stdin().read_line(&mut guess).expect("Failed to read line");

    // 打印变量可直接将变量放在 {} 内
    // 当打印表达式求值结果时, 需要空大括号，并把表达式放外部 println!("{}", x + 1)
    println!("You guessed: {guess}");
}
```

#### 添加依赖

为了用到随机数功能，现在添加一个 rand 库的依赖，在 Cargo.toml 中添加

```ini
[dependencies]
rand = "0.8.5"
```

该说明符0.8.5实际上是 的简写^0.8.5，这意味着任何至少 0.8.5 但低于 0.9.0 的版本

```rs
// Rng 类型是一个定义了生成随机数的接口的 trait
// rand 包提供了 Rng trait 的多个实现，每个实现都提供了一种不同的生成随机数的方式
// 这里 use rand::Rng; 中 Rng 是为了将接口引入作用域, 否则 gen_range 不可见
use rand::Rng;

// thread_rng() 将返回一个 Rng trait 的实现, 从而可以调用 gen_range 方法来获取随机数
let secret_number = rand::thread_rng().gen_range(1..=100);
```

###### 特征 (trait)

trait 用来定义一组接口，该接口可以动态地绑定到各种类型

```rs
trait MyTrait {
    fn mymethod(&self);
}

impl MyTrait for String {
    fn mymethod(&self) {
        std::println!("Hello")
    }
}

fn main() {
    let x: String = String::new();
    x.mymethod();
}
```

##### 本地文档

cargo doc 命令将在本地构建所有依赖项提供的文档并在浏览器中打开它

```sh
cargo doc --open
```

#### 进行比较

```rs
// Ordering 类型是一个枚举，具有变体Less、Greater和Equal
use std::cmp::Ordering;

// 在比较之前需要将输入的字符串转为整型
// Rust 允许我们用新值来掩盖之前的值，这样我们就可以重用 guess 变量名称
// trim() 方法用于去除字符串两端的空白字符（包括空格、制表符、换行符等）, 因为从终端输入会包含换行符
// parse() 方法用于将字符串解析为合适的数据类型，例如将字符串解析为整数、浮点数等
// 通过 :u32 我们指定了字符串将转换为 32 位无符号型
let guess:u32 = guess.trim().parse().expect("Please type a number!");

// cmp 是一个通用方法，可以在任何可以比较的对象上调用
match guess.cmp(&secret_number) {
    Ordering::Less => println!("Too small!"),
    Ordering::Greater => println!("Too big!"),
    Ordering::Equal => println!("You win!"),
}
```


mise 支持工具列表: https://mise.jdx.dev/registry.html

## 安装

### Windows 安装 

`winget install jdx.mise`

mise 安装包很简洁，只有一个 mise.exe 文件, winget 下载后会放到 
C:\Users\shino\AppData\Local\Microsoft\WinGet\Packages
之后, 通过软连接将 exe 放到 Links 文件夹, 而 Links 文件夹被添加到环境变量 PATH 中
C:\Users\shino\AppData\Local\Microsoft\WinGet\Links

mise 工作目录是 C:\Users\shino\AppData\Local\mise

其中 `C:\Users\shino\AppData\Local\mise\shims` 下存放各个工具 use 后的包装脚本
需要导出 PATH 才能访问到安装的工具

而全局配置文件是 `C:\Users\shino\.config\mise\config.toml`, 用于各个工具的全局配置.

## 设计理念

mise 之所以能够动态更改工具版本，是通过导出一个公共文件夹到 PATH, 
然后每添加一个工具就往该文件夹中生成一个和工具名一样名称的包装脚本. 例如 node.cmd

```bat
@echo off
setlocal
mise x -- node %*
```

核心就在于所有安装的可执行的工具都需要通过 mise x 来执行.

这样一来, 不同工作目录就可以自动切换不同的版本了

只要工作目录下包含 mise.toml 文件, 系统或命令行尝试调用该工具时, 会先调用导出的包装脚本，
而该脚本使用 mise x 运行该工具，这就使得 mise 有机会检测工作目录下 mise.toml 并根据情况调用指定版本的工具


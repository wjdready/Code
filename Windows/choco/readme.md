
choco 可用来快速安装各种工具和库

首先获得 choco, 根据[官网](https://chocolatey.org/install)的说明，安装是比较容易的, 通常一个 powershell 命令即可完成 choco 的安装:

```sh
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
```

通过 `where choco` 发现安装位置似乎被放到 `C:\ProgramData\chocolatey`， 根据习惯, 我将其移动到了 `C:\ProgramFiles\Library\chocolatey`, 那么再编写一个 xchoco.bat 按需导出环境

```bat
set PATH=%PATH%;C:\ProgramFiles\Library\chocolatey\bin
```

## 常用包

安装位置可真是一言难尽了，各个包安装位置都不统一。

比如 vim 安装在 `C:\tools\vim` 下, 会创建图标, 然后自动添加 `C:\Windows\vim.bat` 脚本用于从命令行中启动。mingw 则安装在 `C:\ProgramData\mingw64` 中, 安装脚本会将该目录自动导出到 User PATH。


```sh
choco --help

# 查找某个包
choco search package

# vs 开发工具
choco install visualstudio2022-workload-vctools

# msys2, INSTALL C:\tools\msys64, User PATH C:\tools\msys64
choco install msys2
```

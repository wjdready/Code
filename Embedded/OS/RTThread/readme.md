---
mytags: myblog
title: RTThread 开发环境
date: 2023-02-17 11:12:50
categories: [RTThread]
tags: [RTThread, 开发环境]
---

本文主要介绍如何搭建 RTThread 开发的开发环境
<!-- more -->

RTThread 开发支持裸机移植, Env 工具移植, 和 RTThread Studio 直接开发。一般裸机移植只适用于 RTThread Nano, 因为 Nano 不需要各种组件包, 很简洁适用于系统资源十分有限的MCU中, 但这样就发挥不了 RTThread 组件生态的好处了, 通过 Env 工具和 RTThread Studio 开发可以直接获取网络的官方或第三方成熟的组件, 加快开发流程。下面将主要讨论使用 Env 工具开发的基本流程。

#### 准备工作
到[官网下载](https://www.rt-thread.org/page/download.html):

* RTThread 标准版源码
* Env 工具

#### Env 工具的基本使用
参考官方[使用介绍](https://www.rt-thread.org/document/site/#/development-tools/env/env)

安装完成后打开 env.exe, 为了能随时随地打开env.exe方便使用, 将工具添加到注册表(标题栏右击->Settings->Integration->Register)

Env 中携带了 Python & scons 以及 gcc 工具链, 安装完就可以直接编译.

##### 1. 编译 BSP

在源码的 bsp 目录找到适合自己板子的工程模板, 在模板目录下打开 env.exe 输入 `scons` 即可开始编译当前板子的工程代码. 

##### 2.生成工程文件
  如果使用 mdk/iar 来进行项目开发, 可以直接使用 BSP 中的工程文件或者使用以下命令中的其中一种，重新生成工程.
  ```
  scons --target=iar
  scons --target=mdk4
  scons --target=mdk5
  ```

#### 3. menuconfig

输入 `menuconfig` 进行配置, 配置完后后自动生成 rtconfig.h 文件。此时再次使用 scons 命令就会根据新的 rtconfig.h 文件重新编译工程了

#### 4. pkg 包管理器([官方包地址](https://github.com/RT-Thread-packages))

package 工具作为 Env 的组成部分，为开发者提供了软件包的下载、更新、删除等管理功能.

在下载、更新软件包前，需要先在 menuconfig 中 开启 你想要操作的软件包, 这些软件包位于 RT-Thread online packages 菜单下. 找到你需要的软件包然后选中开启，保存并退出 menuconfig 。此时软件包已被标记选中，但是还没有下载到本地，所以还无法使用。

这时候使用 pkg 命令进行下载、更新、删除软件包

```shell
# 1. 下载
# 如果软件包在 menuconfig 已被选中，但是未下载，此时输入如下命令, 该软件包自动下载；
pkgs --update

# 2. 更新
# 如果选中的软件包在服务器端有更新，并且版本号在 menuconfig 中选择的是 latest. 输入如下指令, 该软件包将会进行更新；
pkgs --update

# 3. 删除
# 需要先在 menuconfig 中取消其的选中状态，然后再执行如下指令 。此时本地已下载但未被选中的软件包将会被删除。
pkgs --update

# 4. 升级本地软件包信息
# 这个命令不仅会对本地的包信息进行更新同步，还会对 Env 的功能脚本进行升级，建议定期使用
pkgs --upgrade

```
#### 5. scons 更多功能

参考[官方说明](https://www.rt-thread.org/document/site/#/development-tools/scons/scons)

SCons 基本功能

RT-Thread 构建系统支持多种编译器, 可以阅读该 BSP 目录下的 rtconfig.py 里的 CROSS_TOOL 选项查看当前支持的编译器。

```shell
scons --target=mdk5     # 生成 mdk5 的工程
scons -c                # 清除构建内容
scons -s                # 不输出内部命令信息, 可以连着用比如 scons --target=mdk5 -s
scons -jN               # 多线程编译
scons --dist            # 输出BSP相关工程, 所有 bsp 相关源码都拷贝到 dist 目录下
scons --verbose         # 显示编译参数
```

> 注：要生成 MDK 或者 IAR 的工程文件，前提条件是 BSP 目录存在一个工程模版文件，然后 scons 才会根据这份模版文件加入相关的源码，头文件搜索路径，编译参数，链接参数等。而至于这个工程是针对哪颗芯片的，则直接由这份工程模版文件指定。所以大多数情况下，这个模版文件是一份空的工程文件，用于辅助 SCons 生成 project.uvprojx 或者 project.eww。


#### Env 高级用法

该用法是我独自探索出来的, 实用性还是不错的.

我发现下载的 env 包里面有一个 env.bat, 运行后直接开启一个 ConEmu.exe 终端程序, 其关键内容正是这段启动脚本

```
start  .\tools\ConEmu\ConEmu.exe 
```

ConEmu.exe 是随包下载放在 tools 目录下的. ConEmu.exe 是一个开源的终端模拟器, RTT 官方默认让用户使用该终端来承载他们提供的 env 环境.

然而一般来说我习惯在 VSCode 中使用 VSCode 自带的终端模拟器或者直接使用 Windows Terminal, 但是当我在 VSCode 或 cmd 中启动 RTT env 时, 不论我是使用 env.bat 还是 直接运行 .\tools\ConEmu\ConEmu.exe 它总是开启一个新的窗口, 有没有办法直接在 Windows Terminal 中进入 RTT env 环境呢? 

我想 ConEmu.exe 作为一个终端模拟器, 和 Windows Terminal 本质是一样的, 只是作为承载开发环境的工具, 在进入 ConEmu.exe 的同时, 一定有环境导出的相关脚本，要不然就是官方更改了 ConEmu.exe 源码, 将环境导出内置了, 但这可能性不大。

最终果然在 `.\tools\ConEmu\ConEmu` 目录下找到初始化脚本 `CmdInit.cmd`, 这样一来就方便多了。

例如我可以写一个 `rtt.bat` 放入 Windows 的 PATH 中, 不论我是使用 VSCode 自带的终端模拟器还是 Windows Terminal, 只需输入 rtt 即可进入 RTT env 环境, 或者如果不想进入  RTT env 只是临时调用一下来编译程序只需输入 `rtt scons`, 这一点通过 `CmdInit.cmd & %* & exit"` 来实现, %* 在bat 脚本中是获取所有的输入参数, 即运行 CmdInit.cmd 脚本进入环境后后立刻运行 rtt.bat 其后的参数, 比如 scons, 最后运行完后执行 exit 退出环境. 

rtt.bat 的实现如下:

```bat
@REM 如果输入参数不为空, 就按需执行, 执行完后立刻退出, 否则直接进入 RTT Env 环境

@if not "%*" == "" (
    cmd /k "C:\ProgramFiles\Library\RTThread-env-tools-v1.3.5\tools\ConEmu\ConEmu\CmdInit.cmd & %* & exit"
) else (
    cmd /k "C:\ProgramFiles\Library\RTThread-env-tools-v1.3.5\tools\ConEmu\ConEmu\CmdInit.cmd
)
```


#### RTThread VSCode 插件

最后, 我发现 RTThread 官方已经提供 VSCode 的开发插件, 使用起来更简单了.


## 官方教程

[官方教程](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/README)



## Note 

scons 使用的是 python 2.7, 里面的命令无法支持长路径复制, 因此 scons --dist 如果路径过长就会导出失败, 真的坑...


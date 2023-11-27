# [2023-11-17 10:52:31]

Eclipse 需要强制使用 Workspace, 打开 IDE 首先就是要指定一个文件夹作为 Workspace.

Workspace 主要管理 .metadata 文件夹

安装其实很大，有几百MB. 最恶心的是安装目录很小，安装用到的核心功能插件都放在 home 下的 .p2 和 .eclipse

可以下载一个 installer 然后通过选项自动联网下载对应的开发的功能，也可以一步到位，
在该[网址](https://www.eclipse.org/downloads/packages/)下载 C/C++ 专门的 IDE

## 界面研究

#### Windows

菜单栏上的 Windows 主要用来设置整个 IDE 界面显示和 IDE 的全局配置:

比如 Project Explorer 被关闭了，可在 Show View 中找到并重新打开
New Windows 似乎可以重新打开窗口，并恢复所有为默认状态

项目:

.project 是 Eclipse 工程文件
.cproject 是 CDT 插件的工程文件
.settings 是项目配置目录


#### Project Explorer

右击文件夹 Show in Local Terminal 可以打开终端 (常用)


## 插件

#### CDT

全称 C/C++ Development Tools


## 主题




* 当连接出错时, 比如 undefined reference to `__imp_xxxx`
  查找 [Win32API](https://learn.microsoft.com/zh-cn/windows/win32/api/) 对xxxxx 进行搜索, 会找到其库名称, 然后在编译选项添加即可

* 开始 ssh 服务 net start sshd
  进不去? Permission denied 老火 

## MingW 的安装

虽然有 MSYS2 但是可能会推荐其他人更加小巧的 MingW, 所以还是有必要能够快速下载。

到[官网下载地址](https://www.mingw-w64.org/downloads/), Sources 部分有一个 [SourceForge 链接](https://sourceforge.net/projects/mingw-w64/files/mingw-w64/mingw-w64-release/), 最下面的编译好的

官网下载速度太慢，这个[地址给力](https://www.onlinedown.net/soft/10045442.htm)

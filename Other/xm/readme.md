
## 新 xm 设计理念

启动时读取当前程序路径下的 config.yml 文件, 如果没有文件则提示没有 config.yml 文件 (给出绝对路径).

```yml
# 工具名, 当只执行 xm 时, 会列出所有工具
python:
  # 工具版本, 当执行 xm python 则会列出所有支持的版本, 每个版本有3种状态
  # used 表示该工具已经正常导出到环境，正常使用中, 不管是 http 方式还是本地文件夹路径
  # installed 表示该工具的 links 中包含 http 需要下载的文件, 并且已经下载，但是还没有导出使用
  # local 表示该工具 links 全是本地文件夹路，不需要下载，只需导出即可
  # +-------------+-------------+-------------+
  # |  * : used,  @ : installed,  # : local   |
  # +-------------+-------------+-------------+
  # | * 3.8.10    |  # 3.9.5    |  @ 3.10.10  |
  # +-------------+-------------+-------------+
  # 执行 xm python unset 3.8.10 则会将 3.8.10 从环境中移除, 同时列出时, * 被消除, 如果是本地的，则会替换成 #
  # 执行 xm python remove 3.10.10 移除已经安装的版本, 同时 @ 被消除
  - version: 3.8.10
    # link 可以是http也可以是本地文件夹路径
    links:
      - D:\ProgramFiles\Library\python\python38
      - D:\ProgramFiles\Library\python\python38\Scripts
  - version: 3.9.5
    envs:
      - PYTHONHOME=D:\ProgramFiles\Library\python\python39
    # 可以引用当前版本设置的环境变量
    links:
      - $PYTHONHOME
      - $PYTHONHOME\Scripts
  - version: 3.10.10
    # true 表示自动创建下载解压后的路径的软连接并导出环境变量 (默认为 true)
    # false 时既不创建软连接, 也不导出环境变量
    httpLinkAutoExport: false
    # 将要创建的软连接列表, 如果不存在 paths 属性, 则默认导出 links 中的所有路径
    links:
      # 对于 http link 当执行 xm [toolname] install [version] 会下载到 程序路径/cache/当前工具名/版本名/
      # 下载完成后会解压到 程序路径/install/当前工具名/版本名/文件名/
      - https://github.com/wjdready/release/python310.zip
      # 可以通过 $number 来引用上面的链接自动解压的路径 "程序路径/install/当前工具名/版本名/文件名/"
      # 如果不是 http link 而是本地文件夹路径, 则 $0 则为引用该本地路径  
      - $0\Scripts
    # 如果定义path, 则 links 中列出的软连接, 将不再自动导出, 而是以 paths 为主
    paths: $0\Scripts

node:
  - version: 18.16.0
    # 变量和链接列表只有一个时可以简写
    envs: NODEROOT=D:\ProgramFiles\Library\node\node18.16.0
    links: D:\ProgramFiles\Library\node\node18.16.0
  - version: 19.8.1
    links: https://nodejs.org/dist/v19.8.1/node-v19.8.1-x64.zip 

# 单行模式
python: {version: 3.8.10, links: D:\ProgramFiles\Library\python\python38}
node: {version: 18.16.0, links: D:\ProgramFiles\Library\node\node18.16.0}
```

使用方式:
  xm [选项] [模块名称] [命令]
  -h, --help                   显示帮助信息
  -p                           显示所有模块状态
  -x                           显示当前 PATH 环境变量
  --unset-all                  清理所有模块环境变量
  --export-all                 预先为所有模块导出环境变量, 这样在以后使用时可以直接有效
  -s, --session                会话模式, 只在当前会话有效, 退出后环境变量会被清除
  [module]                     显示可用版本列表
  [module] install [version]   安装指定版本
  [module] use [version]       使用指定版本
  [module] unset               取消使用当前版本
  [module] remove [version]    移除已安装版本


## 包装脚本

需要配合包装脚本, 运行时首先是运行包装脚本，通过包装脚本将 xm 命令的输出结果进行解析, 
输出 ::ENV:: 开头的是变量, 则解析出环境变量, 并将环境变量导出到当前会话中.
否则直接输出.

```bat
@echo off
@chcp 65001 >nul

set "temp_env_file=%temp%\xm_env_%random%.tmp"

for /f "delims=" %%i in ('bin\xm %* 2^>^&1') do (
    setlocal enabledelayedexpansion
    set "line=%%i"
    if "!line:~0,7!"=="::ENV::" (
        set "env_cmd=!line:~7!"
        echo set "!env_cmd!">>"%temp_env_file%"
    ) else (
        echo %%i
    )
    endlocal
)

if exist "%temp_env_file%" (
    for /f "delims=" %%a in ('type "%temp_env_file%"') do (
        %%a
    )
    del "%temp_env_file%" >nul 2>&1
)

set "temp_env_file="
```


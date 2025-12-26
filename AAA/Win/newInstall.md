---
mytags: myblog
title: Windows 安装
date: 2025-12-26 10:59:41
categories: [windows]
tags: [windows install, 手册]
---

Windows 新安装时需要的操作

<!-- more -->


## 基于新安装的

1. User 文件夹移动到D盘

2. 显示扩展名和隐藏文件

3. 添加到环境变量PATH
   D:\ProgramFiles\Library\command
   D:\ProgramFiles\Library\mise
   D:\ProgramFiles\Library\git\bin
   D:\ProgramFiles\Dev\Code
   D:\ProgramFiles\Library\flutter\flutter_current

4. VSCode
   mklink /D "C:\Users\shino\.vscode" "D:\Home\Documents\VSCode\.vscode"
   mklink /D "C:\Users\shino\AppData\Roaming\Code" "D:\Home\Documents\VSCode\Code"
   VSCode 右击菜单修复
   ```re
   Windows Registry Editor Version 5.00

   ; 打开文件
   [HKEY_CLASSES_ROOT\*\shell\VSCode]
   @="Open with Code"
   "Icon"="D:\\ProgramFiles\\Dev\\Code\\Code.exe"

   [HKEY_CLASSES_ROOT\*\shell\VSCode\command]
   @="\"D:\\ProgramFiles\\Dev\\Code\\Code.exe\" \"%1\""

   ; 打开文件夹
   [HKEY_CLASSES_ROOT\Directory\shell\VSCode]
   @="Open with Code"
   "Icon"="D:\\ProgramFiles\\Dev\\Code\\Code.exe"

   [HKEY_CLASSES_ROOT\Directory\shell\VSCode\command]
   @="\"D:\\ProgramFiles\\Dev\\Code\\Code.exe\" \"%V\""

   ; 空白处打开
   [HKEY_CLASSES_ROOT\Directory\Background\shell\VSCode]
   @="Open with Code"
   "Icon"="D:\\ProgramFiles\\Dev\\Code\\Code.exe"

   [HKEY_CLASSES_ROOT\Directory\Background\shell\VSCode\command]
   @="\"D:\\ProgramFiles\\Dev\\Code\\Code.exe\" \"%V\""
   ```

5. 安装最新 pwsh 并设置终端默认为 pwsh
   winget install --id Microsoft.PowerShell --source winget

6. mise 配置
    setx MISE_DATA_DIR "D:\ProgramFiles\Library\mise"
    setx MISE_CONFIG_DIR "D:\ProgramFiles\Library\mise"

7. 设置->系统->开启开发者选项 (flutter编译需要)


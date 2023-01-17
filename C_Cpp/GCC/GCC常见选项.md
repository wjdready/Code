---
mytags: myblog
title: GCC常见选项
date: 2023-01-17 23:07:18
categories: [C_Cpp, GCC]
tags: [编译器, 工具使用]
---

本文主要介绍
<!-- more -->

# GCC常见选项

## 链接

* -Wl,option 用于将特定的选项传给链接器

  例如当我链接程序时，我们希望将-t标志传递给链接器，以便它跟踪链接器处理的输  入文件的名称

  ```sh
  gcc -Wl,-t main.c
  # 输出:
  # C:\ProgramFiles\msys64\tmp\ccAp52pH.o
  # ...
  ```

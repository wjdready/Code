---
mytags: myblog
title: Python 文件批量操作模板
date: 2023-02-15 22:13:56
categories: [Python, 小工具]
tags: [Python, 批量操作]
---

本文主要介绍一个常用的 Python 文件批量操作模板, 用于快速处理日常办公文件操作问题

<!-- more -->

直接看代码

```python
#!python

# 文件批量操作模板
# 导出 exe: pyinstaller --onefile --nowindowed temp.py

import os
import sys

def doProcessFile(filepath):
    print(filepath)

# 处理给定路径
def processPath(path):
    # 文件直接执行
    if os.path.isfile(path):
        doProcessFile(path)
        return 

    # 目录，则递归处理
    for root, dirs, files in os.walk(path):
        for file in files:
            filepath = os.path.join(root, file)
            doProcessFile(filepath)

def main():

    # 通过在程序内输入文件或文件夹路径来执行
    if len(sys.argv) <= 1:
        print("输入文件或目录\n或将文件或目录拖入并按回车键执行\n")
    
    # 输入模式死循环
    while len(sys.argv) <= 1:
        path = input()
        processPath(path)
        print('\nDONE')

    # 否则通过命令行参数输入或拖入文件或文件夹到可执行程序来执行
    for i in range(1, len(sys.argv)):
        processPath(sys.argv[i])

if __name__ == '__main__':
    main()
    print('\nDONE')
    input()
```

程序中包含几个输入模式:
1. 通过在命令行中调用该程序并输入文件或文件夹参数
2. 将脚本打包成 exe 后, 通过将要处理的文件或文件夹拖到程序图标即可执行
3. 打开程序进入等待输入, 然后在输入框中输入, 或者直接将文件或文件夹拖进输入框中按回车执行

### 使用模板

使用该模板也非常简单, 其中只要对 `doProcessFile` 更改一下即可, 例如:
* 批量更改文件编码
  ```python
  # 更改文件编码
  def doProcessFile(filepath):
      try:
          with open(filepath, 'r', encoding = 'gbk') as fr:
              doc = fr.read()
          with open(filepath, 'w', encoding = 'utf-8') as fw:
              fw.write(doc)
      except UnicodeDecodeError:
          print(filepath + " 不是 GBK 编码")
  ```

* 批量更改文件名
  ```python
  # 更改文件名, 将 jpg 改成 png
  def doProcessFile(filepath):
      name, type = os.path.splitext(filepath)
      if type == ".jpg":
          os.rename(filepath, name + ".png")
  ```

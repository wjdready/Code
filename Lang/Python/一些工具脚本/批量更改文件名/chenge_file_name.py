#!python

# 导出 exe: pyinstaller --onefile --nowindowed gbk2utf-8.py

import os
import sys

# 更改文件名, 将 jpg 改成 png
def doProcessFile(filepath):
    name, type = os.path.splitext(filepath)
    if type == ".jpg":
        os.rename(filepath, name + ".png")

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

    # 通过程序内输入文件或文件夹路径来执行
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

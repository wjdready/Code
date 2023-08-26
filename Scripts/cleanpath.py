#!/usr/bin/python3

import os
import sys
import shutil

def doProcessFile(filepath:str):

    name, type = os.path.splitext(filepath)
    basename = os.path.basename(name)

    if not basename == "Makefile" or "/_" in filepath or "/." in filepath:
        return 

    with open(filepath, encoding="UTF-8") as f:
        line = f.readline()

        if not line:
            return

        if not line[0] == "#" or not "[MyProject]" in line:
            return

        dirname = os.path.dirname(filepath)

        command = "make -C " + dirname + " globalclean --silent"
        print("globalclean: " + filepath)
        os.system(command)

        return

# 处理给定路径
def processPath(scanpath:str):
    # if '/' in blogpath:
    #     print(blogpath + " 文件名不能包含 '/' 字符")
    #     return

    if scanpath[-1] == '/':
        scanpath = scanpath[:-1]

    for root, dirs, files in os.walk(scanpath):
        for file in files:
            filepath = os.path.join(root, file)
            doProcessFile(filepath)

def main():

    # 通过在程序内输入文件或文件夹路径来执行
    if len(sys.argv) != 2:
        print("Usage: %s <path_scan> " % (sys.argv[0]))
        exit()

    if not os.path.isdir(sys.argv[1]):
        print("找不到路径: " + sys.argv[1])
        return 

    processPath(sys.argv[1])

if __name__ == '__main__':
    main()

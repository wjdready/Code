#!python

# 文件批量操作模板
# 导出 exe: pyinstaller --onefile --nowindowed jie.py

import os
import sys

def copy_file(file):
    try:
        sf = open(file, 'rb')
    except OSError:
        print('Open file ' + file + ' error')
        return False

    df = open(file + '.bak','wb')
    while True:
        dat = sf.read(4069)
        if not dat:
            break
        df.write(dat)

    sf.close()
    df.close()
    return True

def jie_file(file):
    print(file)
    if copy_file(file):
        try:
            os.remove(file)
            os.rename(file + '.bak', file)

        except Exception as err:
            print('\nFAILED: in file ' + file + ' error')
            print(err)
            input()
            sys.exit()

def doProcessFile(filepath):
    jie_file(filepath)

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
        print("文档解密\n输入文件或目录\n或将文件或目录拖入并按回车键执行\n")
    
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

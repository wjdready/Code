#!python

# 文件批量操作模板
# 导出 exe: pyinstaller --onefile --nowindowed temp.py

import os
import sys

def fileWriteLine(file, dat):
    print(dat)
    file.write(dat)
    file.write('\n')

def processPath(dir):
    output = open("文件列表.txt", "w", encoding='utf-8')

    fileWriteLine(output, "=" * 64)
    fileWriteLine(output, "[PRINT LIST DIR] %s" % dir)
    fileWriteLine(output, "=" * 64)
    for root, dirs, files in os.walk(dir):
        level = root.replace(dir, '').count(os.sep)
        indent = '    ' * level
        fileWriteLine(output, '\n{}[ {} ]'.format(indent, os.path.basename(root)))
        subindent = '    ' * (level + 1)
        for f in files:
            fileWriteLine(output, '{}{}'.format(subindent, f))
    fileWriteLine(output, "=" * 64)
    output.close()

def main():

    # 通过在程序内输入文件或文件夹路径来执行
    if len(sys.argv) <= 1:
        processPath(".")

    # 否则通过命令行参数输入或拖入文件或文件夹到可执行程序来执行
    for i in range(1, len(sys.argv)):
        processPath(sys.argv[i])

if __name__ == '__main__':
    main()
    print('\nDONE')
    input()

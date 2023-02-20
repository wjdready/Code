#!python

# 文件批量操作模板
# 导出 exe: pyinstaller --onefile --nowindowed temp.py

import os
import sys

PIPE = "│"
ELBOW = "└──"
TEE = "├──"
PIPE_PREFIX  = "│   "
SPACE_PREFIX = "    "

def list_files(startpath):

    for root, dirs, files in os.walk(startpath):
        break
    
    tree = []    
    for i, file in enumerate(files):
        if i == len(files)-1 and len(dirs) == 0:
            joint = ELBOW
        else:
            joint = TEE
        tree.append('{} {}'.format(joint, file))
    
    for i, dir in enumerate(dirs):
        if i == len(dirs)-1:
            joint = ELBOW
            space = SPACE_PREFIX
        else:
            joint = TEE
            space = PIPE_PREFIX
        
        tree.append('{} {}/'.format(joint, dir))
        branches = list_files(os.path.join(root,dir))
        for branch in branches:
            tree.append('{}{}'.format(space, branch))

    return tree


def processPath(startpath):

    tree = list_files(startpath)
    string = '../{}/\n'.format(os.path.basename(startpath))

    for t in tree:
        string += '{}\n'.format(t)
    string = string.replace('\n', '\n   ')

    print(string) 
    output = open("文件列表.txt", "w", encoding='utf-8')
    output.write(string)
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

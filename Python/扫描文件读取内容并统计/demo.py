#!python3

# 测试需要测试云平台数据, 故帮其写一个用来快速统计 
# 多个 xml 文件中的有关时间的行并统计时间差

import sys
import os
import re
from datetime import *

# pyinstaller --onefile --nowindowed --icon="path/to/icon" demo.py
# pyinstaller --onefile --nowindowed  demo.py

fileout = 'output.txt'


def get_time_str(seconds):
    h = int(seconds / 3600 % 10000)
    m = int(seconds / 60 % 60)
    s = int(seconds % 60)
    retstr = ''
    if h > 0:
        retstr = retstr + ("%02d 时 " % h)
    if m > 0 or (m == 0 and h != 0 and s != 0):  # 分 > 0 或 1 时 0 分 23 秒 情况
        retstr = retstr + ("%02d 分 " % m)

    if (s == 0 and h != 0) or (s == 0 and m != 0):
        None
    else:
        retstr = retstr + ("%02d 秒" % s)

    return retstr


def get_data_sort_list(data_list):
    listkey = []
    for key in data_list:

        if not key:
            continue

        listkey.append(int(key))

    listkey.sort()
    return listkey

def get_max_len_of_keylist(listkey):
    lenmax = 0
    for key in listkey:
        if not key:
            continue
        lenmax = max(lenmax, len(str(key)))

    return lenmax

def write_to_file(data_list, filename):

    sorted_listkey = get_data_sort_list(data_list)
    width = get_max_len_of_keylist(sorted_listkey)

    file = open(filename, "w", encoding='utf-8')
    file.write(("用时\t开始时间\t结束时间\t原始数据\t所在文件\t第几行\n"))

    print("\n总计:\n\n%-5s %s\n" % ("个数", "时间"))
    for key in sorted_listkey:
        if not key:
            continue
        arr = data_list[str(key)]
        print("[%s] %s" % (str(len(arr)).center(width), get_time_str(int(key))))
        n = 1
        for i in range(0, len(arr)):
            if n == 1:
                file.write(("%s (%d 个)\n" %
                           (get_time_str(int(key)), len(arr))))
            file.write(("\t%s\n" % (arr[i])))
            n = n + 1
        file.write("\n")

    print("\n文档已输出到 %s\n" % os.path.abspath(filename))

    file.close()


def find_data(file_name, data_list):
    print("找到文件: %s" % file_name)
    if not file_name[-4:] == '.xml':
        return
    file = open(file_name, "r", encoding='utf-8')
    n = 1
    while True:
        try:
            line = file.readline()
        except UnicodeDecodeError as reason:
            print("错误: 文件 %s 不是 UTF-8 编码" % file_name)
            break

        if not line:
            break

        regular = re.findall('<Data.*?=.*?=\"([0-9]*)\".*?=\"([0-9]*)\"', line)

        if len(regular) > 0:
            date1 = regular[0][0]
            date2 = regular[0][1]

            time1 = datetime.strptime(date1, "%Y%m%d%H%M%S")
            time2 = datetime.strptime(date2, "%Y%m%d%H%M%S")

            diff = time2 - time1
            output_line = ("%s\t" "%s\t" "%s\t" "%s\t" "%d" % (
                str(time1), str(time2), str(line).strip('\t\n'), file_name, n))
            # print(output_line)

            key = str(diff.seconds + diff.days * 3600)

            if not key in data_list.keys():
                data_list[key] = []

            data_list[key].append(output_line)
        n = n + 1

    file.close()


def scan_dir(path, data_list):
    for root, dirs, files in os.walk(path):
        for file in files:
            filepath = os.path.join(root, file)
            find_data(filepath, data_list)

# 输入模式


def input_mode():
    print("输入要扫描的文件或目录\n或将文件或目录拖入并按回车键进行扫描\n")
    while True:
        path = input()
        if len(path) > 1:
            data_list = {'': []}
            # 目录，则递归查找
            if os.path.isdir(path):
                scan_dir(path, data_list)
            else:
                find_data(path, data_list)
            print('\nDONE')
            write_to_file(data_list, fileout)


def main():
    arg_len = len(sys.argv)
    if arg_len <= 1:
        input_mode()

    # 通过命令行参数输入
    data_list = {'': []}

    for i in range(1, arg_len):
        path = sys.argv[i]
        if os.path.isdir(path):
            scan_dir(path, data_list)
        else:
            find_data(path, data_list)

    write_to_file(data_list, fileout)


if __name__ == '__main__':
    main()
    print('\nDONE')
    print('\n\n***** 按回车打开文件 *****\n\n')
    input()
    os.system("notepad " + os.path.abspath(fileout))

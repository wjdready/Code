#!python3

import re
import sys
import os

def main(source_file, header_file):
    
    cfile = open(source_file, "r", encoding='utf-8')
    hfile = open(header_file, "r", encoding='utf-8')

    cdat = cfile.read()
    hdat = hfile.read()

    # 事先删除干扰声明
    cdat = re.compile(r"^.*\);", re.MULTILINE).sub("", cdat)

    # 替换掉 { }
    cdat = re.compile(r"\)\n^\{[\W\w]*?^\}", re.MULTILINE).sub(");", cdat)

    # 查找 .c 文件中的非静态函数和注释
    func_matches = re.findall(r"^\/\*(?:(?!/\*|\*/)[\w\W])*?\*\/\n+^(?!static).*\);", cdat, re.MULTILINE)
    nmatches = len(func_matches)

    # 替换规则: 将找到的函数放到区域中间
    replacer = "/* -- function prototypes -- */\n\n"
    for m in func_matches:
        replacer += str(m) + "\n\n"
    replacer += "/* -- END OF function prototypes -- */"

    if len(func_matches) == 0:
        return

    # 查找头文件中放置函数声明的区域
    regexstr_area = r"(\/\* -- function prototypes -- \*\/"         \
                    r"[\w\W]+?"                                     \
                    r"\/\* -- END OF function prototypes -- \*\/)"
    
    newtext = ''
    if len(re.findall(regexstr_area, hdat, re.MULTILINE)) > 0:
        hfile = open(header_file, "w", encoding='utf-8')
        newtext = re.compile(regexstr_area).sub(replacer, hdat)

    elif len(re.findall(r"^(#endif[ \n]*)$\Z", hdat, re.MULTILINE)) > 0:
        hfile = open(header_file, "w", encoding='utf-8')
        replacer += r"\n\n\1"
        newtext = re.compile(r"^(#endif[ \n]*)$\Z", re.MULTILINE).sub(replacer, hdat)

    else:
        hfile = open(header_file, "a+", encoding='utf-8')
        newtext = replacer + "\n"
    
    hfile.write(newtext)
    
    hfile.close()
    cfile.close()


if __name__ == '__main__':

    if len(sys.argv) != 3:
        print("Description:\n    Output function declarations and comments to header files according to the current C language source files")
        print("Usage:\n    %s <source_file> <header_file>" % os.path.basename(sys.argv[0]))
        exit(0)
    
    main(sys.argv[1], sys.argv[2])

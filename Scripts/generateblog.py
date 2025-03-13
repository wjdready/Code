#!/usr/bin/python3

import os
import sys
import shutil

def doProcessFile(filepath:str, post_basedir:str, blogindex:int):
    
    name, type = os.path.splitext(filepath)
    if not type == ".md" or "/_" in filepath or "/." in filepath:
        return False
    
    # Windows Python 路径情况
    if "\_" in filepath or "\." in filepath:
        return False

    with open(filepath, encoding="UTF-8") as f:
        line = f.readline()
        if "---" not in line:
            return False
        line = f.readline()
        if not "mytags" in line or not "myblog" in line:
            return False
        
        post_dir = post_basedir + '/' + str(blogindex)
        os.makedirs(post_dir)
        post_dst_file = post_dir + '/' + os.path.basename(filepath)

        shutil.copyfile(filepath, post_dst_file)

        blog_source_link="https://gitee.com/wjundong/code/blob/master"
        
        with open(post_dst_file, "a", encoding="UTF-8") as myfile:
            myfile.write("\n\n[源文件来自于](%s/%s)\n" % (blog_source_link, filepath))
        
        print("find: " + filepath)

        assetpath = os.path.dirname(filepath) + "/" + os.path.basename(name)
        if os.path.exists(assetpath):
            print("copy: " + assetpath)
            shutil.copytree(assetpath, post_dir + "/" + os.path.basename(name))
        

        return 1

# 处理给定路径
def processPath(scanpath:str, blogpath:str):
    # if '/' in blogpath:
    #     print(blogpath + " 文件名不能包含 '/' 字符")
    #     return

    if scanpath[-1] == '/':
        scanpath = scanpath[:-1]

    if blogpath[-1] == '/':
        blogpath = blogpath[:-1]

    blogindex = 0
    post_basedir = blogpath + "/source/_posts/auto"

    if os.path.exists(post_basedir):
        shutil.rmtree(post_basedir)

    for root, dirs, files in os.walk(scanpath):
        for file in files:
            filepath = os.path.join(root, file)
            if doProcessFile(filepath, post_basedir, blogindex):
                blogindex = blogindex + 1

def main():

    # 通过在程序内输入文件或文件夹路径来执行
    if len(sys.argv) != 3:
        print("Usage: %s <path_scan> <dirname_for_blog_sources>." % (sys.argv[0]))
        exit()

    if not os.path.isdir(sys.argv[1]):
        print("找不到路径: " + sys.argv[1])
        return 
    
    if not os.path.isdir(sys.argv[2]):
        print("找不到路径: " + sys.argv[2])
        return

    processPath(sys.argv[1], sys.argv[2])

if __name__ == '__main__':
    main()

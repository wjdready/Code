#!/bin/bash


if [ "$1" ]; then
    exec 3> $1  # 将文件描述符 3 输出重定向到文件
else
    exec 3>& 1  # 将文件描述符 3 输出重定向到标准输出
fi

echo "---"                  >& 3
echo "mytags: myblog"       >& 3
echo "title: "              >& 3
echo "date: $(date '+%Y-%m-%d %H:%M:%S')"   >& 3
echo "categories: "         >& 3
echo "tags: "               >& 3
echo "---"                  >& 3


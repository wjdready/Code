---
mytags: myblog
title: My Shell Tools
date: 2023-01-30 21:30:41
categories: [Shell]
tags: [工具]
---

本文主要介绍一些自己常用的作为工具的shell
<!-- more -->

首先为了方便使用 shell, 在 home 中创建一个文件夹 .myshell, 里面有各种工具脚本, 然后在 .bashrc 中创建一个扫描函数, 对自己的工具脚本文件进行 source.

在 .bashrc 中添加
```shell
for file in $(find ~/.myshell/ -type f -name "*.sh"); do
    head_title=$(head -n 1 $file)
    if [ "$head_title" == "# [source]" ]; then
        source $file
done
```

在 ~/.myshell/demo.sh

```shell
# [source]

function print_something
{
    echo "$1"
}
```

[source] 表示希望该文本自动导出

### 一些 functions

* show_path 编译时有时候 gcc 给出错误的路径不是完整的, 导致 VSCode 无法直接获取对应的字符, 这时通过管道，将编译器的输出给脚本，对报错行添加路径前缀即可
    ```shell
    function show_path
    {
        if [ "$1" ]; then
            prefix=$(echo $(pwd -W)/$1 | sed 's/\//\\\\/g')
        else
            prefix=$(echo $(pwd -W) | sed 's/\//\\\\/g')
        fi

        while read line; do
            echo "$line" |  sed 's/\(.*.c:[0-9]*:[0-9]*\):/'$prefix'\/\1/g'
        done
    }
    ```

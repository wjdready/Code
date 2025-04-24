---
mytags: myblog
title: Windows自动查找程序dll依赖
date: 2023-02-18 11:17:24
categories: [Windows]
tags: [小工具]
---

本文主要介绍在Windows开发中如何自动查找程序的dll依赖
<!-- more -->

Windows 程序在发布的时候往往需要很多第三方 dll 依赖, 而这些依赖一般都在开发环境中, 需要一个一个拷贝到程序目录中, 比较的麻烦. 有一些开发平台会提供程序的打包命令, 当用户执行打包命令时会自动将所有依赖拷贝到程序所在目录中, 例如: QT 的 `windeployqt.exe` 工具, 但你会发现 windeployqt 有时候并没有能够给出所有依赖, 它只是给出了与 QT 自身相关的依赖, 如果你使用了非 QT 的第三方库, 则 windeployqt 可能也找不到.

不过有一个较为实用的技巧是直接将开发环境中bin目录下的所有文件都拷贝到程序目录中，然后运行该程序, 程序运行后选择所有之前拷贝的文件并将其删除, 能够删掉的说明不是该程序的依赖, 最后留下的一定就是当前程序的依赖了, 如此一来程序的所有依赖就都凑齐了.

当然上面的技巧是一种比较笨的办法, 下面介绍的脚本可以较为方便地尽量凑齐所有依赖, 当然除了个别比较特殊难找的依赖外, 如果实在找不到那就只能手动复制了.

find_dll.sh

```shell
#!bash

notfounddlls='KERNEL32.dll'
dllbase=/mingw64/bin/

# @breif 对给定路径下的可执行文件进行依赖查找和拷贝
# @param $1 可执行文件所在的路径
# @return none
function find_dll
{
    if [ "$1" == "" ]; then
        return
    fi

    local path=$1

    nc=1
    while [ $nc -gt 0 ]; do
        nc=0
        # 遍历给定目录下的所有 .exe 和 .dll 文件
        for f in $path/*.exe $path/*.dll; do
            # 遍历当前 .exe 和 .dll 文件的所有 dll 依赖
            for dep in $(strings $f | grep -i '\.dll$'); do
                # 当前路径下还没有该 dll 依赖, 则进行处理
                if [ ! -e $path/$dep ]; then
                    # 判断当前依赖是否是在 notfounddlls 列表中
                    # 在列表中就说明之前找过, 但是没有找打, 因此也不必再找了
                    echo $notfounddlls | grep -iw $dep >/dev/null
                    if [ $? -ne 0 ]; then
                        # 查找 dll 库是否有该 dll 依赖, 有则拷贝到给定目录下
                        # 没有就放到 notfounddlls 列表
                        dllloc=$(find $dllbase -iname $dep)
                        if [ ! -z "$dllloc" ]; then
                            cp $dllloc $path
                            echo "Copying "$(basename $dllloc)
                            nc=$(($nc + 1))
                        else
                            notfounddlls="$notfounddlls $dep"
                        fi
                    fi
                fi
            done
        done
    done

    echo "System DLLS: "$notfounddlls
}

# main 函数
function main
{
    local path=$PWD

    if [ "$1" == "-h" ]; then
        echo "Usage: $0 [path]"
        echo "    Find all dll dependencies of an executable file under a given <path>."
        echo "    If no path parameter is given, the current call path is used."
        return
    elif [ "$1" != "" ]; then
        path=$path/$1
    fi

    find_dll $path
}

main $@
```

V2?

```sh
#!/bin/bash

# 系统DLL白名单(无需复制)
system_dlls='KERNEL32.dll|USER32.dll|ADVAPI32.dll|WS2_32.dll|GDI32.dll|SHELL32.dll|OLE32.dll|OPENGL32.dll|api-ms-win-.*'

# 搜索路径(根据CMakeLists.txt中的设置调整)
dllbase=(
    "/mingw64/bin"
    "/usr/x86_64-w64-mingw32/bin"
    "C:/Program Files/CURL/bin"
    "C:/Program Files (x86)/CURL/bin"
    "${MINGW_PREFIX}/bin"
)

function find_dll {
    [ -z "$1" ] && return
    
    local path="$1"
    local nc=1
    
    echo "正在分析目录: $path"
    
    while [ $nc -gt 0 ]; do
        nc=0
        for f in "$path"/*.{exe,dll}; do
            [ ! -f "$f" ] && continue
            
            echo "分析文件: $(basename "$f")"
            
            # 使用更可靠的依赖分析方式
            for dep in $(ldd "$f" 2>/dev/null | grep -i '\.dll' | awk '{print $3}'); do
                [ -z "$dep" ] && continue
                
                local dllname=$(basename "$dep" | tr '[:upper:]' '[:lower:]')
                echo "检查依赖: $dllname"
                
                # 跳过系统DLL
                echo "$dllname" | grep -Eiw "$system_dlls" >/dev/null && continue
                [ -f "$path/dll/$dllname" ] && continue
                
                # 在搜索路径中查找DLL
                for base in "${dllbase[@]}"; do
                    if [ -f "$base/$dllname" ]; then
                        echo "复制: $base/$dllname -> $path/dll/"
                        mkdir -p "$path/dll"
                        cp -v "$base/$dllname" "$path/dll/"
                        nc=$((nc + 1))
                        break
                    fi
                done
            done
        done
    done
}

function main {
    local path="${1:-$PWD}"
    [ ! -d "$path" ] && echo "目录不存在: $path" && exit 1
    
    echo "开始处理目录: $path"
    find_dll "$path"
    
    # 检查结果
    if [ -d "$path/dll" ]; then
        echo "找到的DLL:"
        ls -l "$path/dll"
    else
        echo "警告: 没有找到任何DLL依赖"
    fi
}

main "$@"
```

#### 参考文献

[Finding DLLs required of a Win exe on Linux (cross-compiled with mingw)?](https://stackoverflow.com/questions/11703000/finding-dlls-required-of-a-win-exe-on-linux-cross-compiled-with-mingw)

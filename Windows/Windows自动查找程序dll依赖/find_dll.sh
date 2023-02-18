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


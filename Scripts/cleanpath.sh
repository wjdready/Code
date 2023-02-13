#!/bin/bash

FULLCLEAN_PATH=$PWD

if [ "$1" ]; then
    FULLCLEAN_PATH=$1
fi

echo "Full Clean Path: $FULLCLEAN_PATH"

IFS=$'\n'   # 仅使用 \n 分割 file list 以支持文件或文件夹的空格名称
for file in $(find $FULLCLEAN_PATH -type f -name "Makefile"); do
    # echo "file $(basename $file) path $file" 

    # 略过 . _ 开头的文件或文件夹
    echo $file | grep -q "^.*/[_.].*" && continue

    head_title=$(head -n 1 $file)
    if [ "$head_title" == "# [MyProject]" ]; then
        make -C $(dirname $file) globalclean --silent || echo -e "       : $file\n"
    else
        echo pass: $file
    fi
done


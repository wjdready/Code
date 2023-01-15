#!/bin/bash

FULLCLEAN_PATH=$PWD

if [ "$1" ]; then
    FULLCLEAN_PATH=$1
fi

echo "Full Clean Path: $FULLCLEAN_PATH"

for file in $(find $FULLCLEAN_PATH -type f -name "Makefile"); do
    # echo "file $(basename $file) path $file" 
    head_title=$(head -n 1 $file)
    if [ "$head_title" == "# [MyProject]" ]; then
        make -C $(dirname $file) fullclean --silent || echo -e "       : $file\n"
    else
        echo pass: $file
    fi
done


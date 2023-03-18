#!/bin/bash

function main {
    [ "$1" == "" ] && exit

    s1=$(echo $1 | sed 's/\\/\//g')
    echo $s1 # | sed 's/C:/\/c/g'
}

if [ "$1" == "-h" ]; then
    echo "Win path to Unix path"
    exit
fi

main $@

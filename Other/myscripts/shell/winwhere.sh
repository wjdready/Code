#!/bin/bash


function main {
    [ "$1" == "" ] && exit 

    s1=$(where $1 | sed 's/\\/\//g')
    echo $s1 # | sed 's/C:/\/c/g'
}

if [ "$1" == "-h" ]; then
    echo "Where command to out Unix Path"
    exit
fi

main $@

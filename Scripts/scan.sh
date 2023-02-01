#!/bin/bash


for file in $(find ~/.myshell/ -type f -name "*.sh"); do
    head_title=$(head -n 1 $file)
    if [ "$head_title" == "# [source]" ]; then
        source $file
done


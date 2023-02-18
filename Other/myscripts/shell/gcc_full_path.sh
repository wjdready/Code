#!bash

# make 2>&1 | show_path simavr

function myshell_gcc_show_path
{
    if [ "$1" == "-h" ]; then
        echo "Description:"
        echo "    Attach full path to the gcc compilation output"
        echo "Usage example:"
        echo "    make 2>&1 | myshell_gcc_show_path [path_prefix]"
        return

    elif [ "$1" ]; then
        prefix=$(echo $(pwd -W)/$1 | sed 's/\//\\\\/g')

    else
        prefix=$(echo $(pwd -W) | sed 's/\//\\\\/g')
    fi

    while read line; do
        echo "$line" |  sed 's/\(.*.c:[0-9]*:[0-9]*\):/'$prefix'\/\1/g'
    done
}

myshell_gcc_show_path $@

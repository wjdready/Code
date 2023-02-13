#!/bin/bash

ROOT_PATH=$PWD
BLOG_DIR_NAME=
BLOG_POSTS_PATH=
BLOG_THEME=keep
BLOG_SOURCE_LINK="https://gitee.com/wjundong/code/blob/master"
index=0

function mylog
{
    if    [ "$1" == "info" ]; then
        echo -e "\033[33;34m"${@:2}"\033[m"
    elif  [ "$1" == "error" ]; then 
        echo -e "\033[33;31m"${@:2}"\033[m"
    elif  [ "$1" == "warning" ]; then
        echo -e "\033[33;33m"${@:2}"\033[m"
    else
        echo ${@:2}
    fi
}

function loop_for_markdown
{
    if [ "$1" == "" ]; then
        return
    fi

    local dir=$1

    IFS=$'\n'   # 仅使用 \n 分割 file list 以支持文件或文件夹的空格名称
    for file in $(find $dir -type f -name "*.md"); do
        # 略过 . _ 开头的文件或文件夹
        echo $file | grep -q "^.*/[_.].*" && continue

        local head_title=$(sed -n '2p' $file)

        if echo "$head_title" | grep -q "^[ ]*mytags[ ]*:[ ]*myblog[ ]*$"; then
            mylog info "find $file" 

            blog_dist_file=$BLOG_POSTS_PATH/auto_generate_$index.md
            cp $file $blog_dist_file

            echo -e "\n\n[源文件来自于]($BLOG_SOURCE_LINK/$file)\n" >> $blog_dist_file

            (( index = index + 1 ))

        else
            # echo pass: $file
            true
        fi
    done
}

function blog_generate
{
    cd $BLOG_DIR_NAME

    command -v npm &> /dev/null || { mylog error "未找到命令 npm"; exit 1; }

    npm install

    if ! command -v hexo &> /dev/null; then
        npm install -g hexo-cli
    fi

    # 主题文件夹为空
    if [ -z "$(ls -A themes/$BLOG_THEME)" ]; then
        git clone https://github.com/XPoet/hexo-theme-keep themes/$BLOG_THEME
    fi

    hexo g

    cd ..

    mylog info "\nDONE"
}

function main
{
    if [ $# -ne 2  ]; then
        echo "Usage: $0 <path_scan> <dirname_for_blog_sources>"
        return
    fi

    [[ -d "$1" ]]       || { mylog error "路径 $1 不存在"; exit -1; }
    [[ "$2" != *'/'* ]] || { mylog error "文件名不能包含 '/' 字符"; exit -1; }
    
    BLOG_DIR_NAME=$2
    ROOT_PATH=$(echo $1 | sed 's/\/$//')    # 去除路径最后的 / 字符
    BLOG_POSTS_PATH=$ROOT_PATH/$BLOG_DIR_NAME/source/_posts/auto

    if [ ! -d "$BLOG_DIR_NAME" ]; then 
        git clone https://github.com/wjdready/myblog.git $ROOT_PATH/$BLOG_DIR_NAME
    fi
    
    mkdir -p $BLOG_POSTS_PATH
    for dir in $(find $ROOT_PATH -maxdepth 1 -type d); do
        local base=$(basename $dir)

        # printf "dir %-16s -- path %s\n" $base $dir
        # 略过 . _ 开头的文件夹
        if [ ${base:0:1} == "." -o ${base:0:1} == "_" -o $base == "$BLOG_DIR_NAME" ]; then 
            mylog warning "pass dir: $dir"
            continue
        fi

        loop_for_markdown $base

    done

    blog_generate
}

main $@


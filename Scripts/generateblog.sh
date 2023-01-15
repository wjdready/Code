#!/bin/bash

ROOT_PATH=$PWD
BLOG_PATH=_myblog
BLOG_POSTS_PATH=$BLOG_PATH/source/_posts/auto
BLOG_THEME=keep

index=0

function loop_for_markdown
{
    if [ "$1" == "" ]; then
        return
    fi

    DIR=$1
    for file in $(find $DIR -type f -name "*.md"); do
        head_title=$(head -n 1 $file)
        if [ "$head_title" == "<!-- myblog -->" ]; then
            echo "file $(basename $file) path $file" 
            blog_dist_file=$BLOG_POSTS_PATH/auto_generate_$index.md

            cp $file $blog_dist_file
            source_link="https://gitee.com/wjundong/code/blob/master/"$file

            echo -e "\n\n[源文件来自于]($source_link)\n" >> $blog_dist_file
            (( index = index + 1 ))
            
            # make -C $(dirname $file) fullclean --silent || echo -e "       : $file\n"
        else
            echo pass: $file
        fi
    done
}

function blog_generate
{
    cd $BLOG_PATH

    command -v npm &> /dev/null || { echo "未找到命令 npm"; exit 1; }

    npm install

    if ! command -v hexo &> /dev/null; then
        npm install -g hexo-cli
    fi

    # 主题文件夹为空
    if [ -z "$(ls -A themes/$BLOG_THEME)" ]; then
        git clone https://github.com/XPoet/hexo-theme-keep themes/$BLOG_THEME
    fi

    hexo g
    hexo s

    cd ..

    echo "DONE"
}

function main
{
    if [ "$1" ]; then
        ROOT_PATH=$1
    fi
    
    if [ ! -d "$BLOG_PATH" ]; then 
        git clone https://github.com/wjdready/myblog.git $BLOG_PATH
    fi
    
    mkdir -p $BLOG_POSTS_PATH

    for dir in $(find $ROOT_PATH -maxdepth 1 -type d); do
        base=$(basename $dir)
        if [ ${base:0:1} != "." -a ${base:0:1} != "_" ]; then 
            # echo "dir $base path $dir"
            loop_for_markdown $base
        fi
    done

    blog_generate
}

main $1


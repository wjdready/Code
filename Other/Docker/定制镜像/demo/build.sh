#!/bin/bash

function download_sync()
{
    export REPO_URL=https://mirrors.tuna.tsinghua.edu.cn/git/git-repo

    if [ ! -f "repo.py" ]; then
        curl https://mirrors.tuna.tsinghua.edu.cn/git/git-repo -o repo.py
        python3 repo.py init -u https://aosp.tuna.tsinghua.edu.cn/platform/manifest -b android-10.0.0_r41 --depth 1 --config < /dev/null
    fi

    python3 repo.py sync
}

function main()
{
    mkdir android -p
    cd android
    download_sync
    
    # 编译
    # source build/envsetup.sh 
    # lunch aosp_arm-eng
    # make -j8
}

main

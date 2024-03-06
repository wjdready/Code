

安卓开发入门 https://source.android.com/docs/setup

## 系统开发

```sh
#!/bin/bash

function download()
{
    export REPO_URL=https://mirrors.tuna.tsinghua.edu.cn/git/git-repo

    if [ ! -f "repo.py" ]; then
        curl https://mirrors.tuna.tsinghua.edu.cn/git/git-repo -o repo.py
        python3 repo.py init -u https://aosp.tuna.tsinghua.edu.cn/kernel/manifest -b android-10.0.0_r41 --depth 1 --config
    fi

    python3 repo.py sync
}

function mybuild()
{
    source build/envsetup.sh 
    lunch aosp_x86_64-eng
    make -j8
}

function myrun()
{
    source build/envsetup.sh 
    lunch aosp_x86_64-eng
    emulator
}
```

构建成功后在 `out/target/product/generic_arch/` 生成各个系统镜像，主要是 `system.img`，`userdata.img` 和 `ramdisk.img`

之后运行 emulator 会自动加载模拟器，emulator 是 prebuilts 的一部分，里面包含了 kernel，加载时会根据 lunch 的架构自动加载对应的 qemu-system-arch

## 内核开发

参考官方文档 https://source.android.com/docs/core/architecture

```sh

mkdir android-kernel && cd android-kernel

export REPO_URL=https://mirrors.tuna.tsinghua.edu.cn/git/git-repo
curl https://mirrors.tuna.tsinghua.edu.cn/git/git-repo -o repo.py

python3 repo.py init -u https://mirrors.tuna.tsinghua.edu.cn/git/AOSP/kernel/manifest -b common-android-4.19-stable --depth 1
python3 repo.py sync
```


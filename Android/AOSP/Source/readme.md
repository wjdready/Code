

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


## Ubuntu22 编译 Android7.1.2

1. 安装 jdk

sudo apt-get install openjdk-8-jdk

2. flex-2.5.39: loadlocale.c:130: _nl_intern_locale_data: Assertion `cnt < (sizeof (_nl_value_type_LC_TIME) / sizeof (_nl_value_type_LC_TIME[0]))' failed.

参考: https://article.juejin.cn/post/7230026709482618917

export LC_ALL=C

3. /bin/bash: line 1: prebuilts/misc/linux-x86/bison/bison: No such file or directory

参考: https://blog.csdn.net/uestcyms/article/details/50384779

sudo apt-get install g++-multilib gcc-multilib

4. /bin/bash: line 1: xmllint: command not found

参考: https://blog.csdn.net/llxxhm/article/details/82428117

sudo apt-get  install libxml2-utils

5. 一直卡住不动，然后报错 SSL error when connecting to the Jack server. Try 'jack-diagnose'

参考: http://www.atmcu.com/2313.html

openjdk-8 包的问题，删除/etc/java-8-openjdk/security/java.security 文件里jdk.tls.disabledAlgorithms 的 TLSv1, TLSv1.1

然后杀死当前卡住的服务 prebuilts/sdk/tools/jack-admin kill-server 重新再执行编译

6. system.img 太大

修改 BoardConfig.mk 中的 BOARD_SYSTEMIMAGE_PARTITION_SIZE


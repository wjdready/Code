

```sh
# ssh 免密登录 .ssh/authorized_keys

# git 配置
git config --global user.name "your_username"
git config --global user.email "your_email"

# pip
curl http://mirrors.aliyun.com/pypi/get-pip.py
pip install some-package -i https://pypi.tuna.tsinghua.edu.cn/simple
pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple

# 系统时间同步网络时钟, 然后同步到硬件时钟
sudo ntpdate cn.pool.ntp.org
sudo hwclock --systohc

# 安卓 repo
curl https://mirrors.tuna.tsinghua.edu.cn/git/git-repo -o repo
export REPO_URL='https://mirrors.tuna.tsinghua.edu.cn/git/git-repo'

# VSCode CLI 启动服务和端口转发, 这样就可用外网连接内网 http 服务了
./code tunnel
socat TCP-LISTEN:8081,fork,reuseaddr TCP:192.168.0.244:80
socat TCP-LISTEN:8082,fork,reuseaddr TCP:192.168.0.244:10101
git clone http://weijundong@192.168.0.244:10101/r/xxxx.git

######################## Flutter #######################
# http://bbs.itying.com/topic/638ad9b23fd95910e036af7f
FLUTTER_STORAGE_BASE_URL=https://storage.flutter-io.cn
PUB_HOSTED_URL=https://pub.flutter-io.cn
# 清华镜像
git clone -b master https://mirrors.tuna.tsinghua.edu.cn/git/flutter-sdk.git
FLUTTER_STORAGE_BASE_URL=https://mirrors.tuna.tsinghua.edu.cn/flutter
PUB_HOSTED_URL=https://mirrors.tuna.tsinghua.edu.cn/dart-pub
# Linux 需要安装的依赖
apt install git curl unzip cmake pkg-config ninja-build clang  libgtk-3-dev
```


## NPM

```sh
# 设置镜像, 恢复回默认, 获取当前镜像
npm config set registry https://registry.npmmirror.com/
npm config set registry https://registry.npmjs.org
npm get registry

# --registry=https://mirrors.cloud.tencent.com/npm/ 腾讯镜像源
# --verbose 可以看到详细的日志

# 或者直接使用 cnpm
npm install -g cnpm --registry=https://registry.npmmirror.com

# 安装 cgr 来快速切换源
npm install -g cgr
cgr ls
cgr use taobao

# 查看某个包的历史版本
npm view <package-name> versions
npm info <package-name>
```

## 配置 Gradle 镜像源

gradle 添加下载地址前缀 `mirrors.huaweicloud.com/gradle`

flutter 目前配置

distributionUrl=https\://mirrors.huaweicloud.com/gradle/gradle-8.10.2-all.zip


```groovy

allprojects {
    repositories {
        maven { url 'https://maven.aliyun.com/repository/central/' }
        maven { url 'https://maven.aliyun.com/repository/google/' }
        maven { url 'https://maven.aliyun.com/repository/public/' }
        maven { url 'https://maven.aliyun.com/repository/gradle-plugin/' }
        mavenLocal()
        // ... 各个模块里面的 build.gradle 里面 implementation 之类的依赖, 就是从这里的仓库查找的.
    }
}

buildscript {
    repositories {
        maven { url 'https://maven.aliyun.com/repository/public/' }
        maven { url 'https://maven.aliyun.com/repository/google/' }
        maven { url 'https://maven.aliyun.com/repository/central/' }
        maven { url 'https://maven.aliyun.com/repository/gradle-plugin/' }
        mavenLocal()
        // ...
    }
    dependencies {
        // ... 这里的库是就是从buildscript里的仓库找的.
    }
}
```

build.gradle.kts

```kt
repositories {
    maven { url = uri("https://maven.aliyun.com/repository/public") }
    maven { url = uri("https://maven.aliyun.com/repository/google") }
    maven { url = uri("https://maven.aliyun.com/repository/central") }
    maven { url = uri("https://maven.aliyun.com/repository/gradle-plugin") }
    mavenCentral()
}
```

## git 自动替换

```sh
# 配置git自动替换 https://gh-proxy.com/github.com
git config --global url.https://gh-proxy.com/github.com.insteadOf https://github.com

# 查看git配置信息
git config --global --list
# 取消设置
git config --global --unset url.https://gh-proxy.com/github.com.insteadOf

GIT_AUTHOR_DATE="2025-07-21 16:13:23" GIT_COMMITTER_DATE="2025-07-21 16:13:23" git commit

```

## cmake

```sh
find_package(SDL2 REQUIRED)

if (SDL2_FOUND)
    message(STATUS "SDL2_LIBRARY: ${SDL2_LIBRARY}")
    message(STATUS "SDL2_INCLUDE_DIR: ${SDL2_INCLUDE_DIR}")
endif()

# For windows release
cmake --build build --config Release
```


## mise

```sh
# 激活环境
mise activate pwsh  | Out-String | Invoke-Expression

# 添加自定插件 file
mise plugin install file https://github.com/wjdready/mise_file_backend_plugin.git
mise link file:vcpkg@current --force D:\ProgramFiles\Library\vcpkg
mise ls
mise use file:vcpkg@current
```

## 必知

```sh
# 1. linux 显示进程网络使用情况
nethogs

# 2. windows 修改终端编码
chcp 65001
chcp 936

# 3. windows 查看文件被哪个进程占用
# 按 Win 键搜 `资源监视器` 并打开, 切换到 CPU 栏搜索文件名即可
```

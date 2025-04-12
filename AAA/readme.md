

```sh
# ssh 免密登录 .ssh/authorized_keys

# git 配置
git config --global user.name "your_username"
git config --global user.email "your_email"

# pip 临时镜像源
pip install some-package -i https://pypi.tuna.tsinghua.edu.cn/simple

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
```



## 配置 Gradle 镜像源

gradle 添加下载地址前缀 `mirrors.huaweicloud.com/gradle`

参考: https://www.cnblogs.com/Chary/articles/18657340

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
# 配置git自动替换
git config --global url."https://gh-proxy.com/github.com".insteadOf https://github.com 

# 查看git配置信息 
git config --global --list 
# 取消设置 
git config --global --unset url."https://gh-proxy.com/github.com".insteadOf
```

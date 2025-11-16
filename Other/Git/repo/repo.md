

# repo 工具原理与实践指南

repo 是 Google 开发的用于管理多个 Git 仓库的工具，主要用于 Android 源码管理，但也可用于其他多仓库项目。

repo 基本原理

清单文件(manifest.xml)：定义所有子仓库的组成结构
主仓库(.repo/manifests)：存储清单文件
子仓库(.repo/projects)：各个独立的 Git 仓库

repo 脚本：Python 编写的管理工具


#### 1. 创建清单项目

```sh
# 创建一个清单项目, 用于汇总各个项目
cd myproject-manifest
git init
git add default.xml
git commit -m "init"

# 然后把 myproject-manifest 上传到远程仓库比如 github
# 如果使用本地则不用 push
git push --set-upstream  origin master
```

* default.xml 文件如下

```xml
<?xml version="1.0" encoding="UTF-8"?>
<manifest>

    <!-- 定义 manifest 的远程仓库路径, 如果使用本地会忽略 fetch的值, 但必须有 -->
    <remote name="mymanifest" fetch="https://github.com" />

    <remote name="github" fetch="https://github.com" />

    <!-- 默认项目, 即 manifest 本身  -->
    <default remote="mymanifest" revision="master" sync-j="4" />

    <!-- 项目列表, remote 指定远程仓库, revision 可指定分支或标签 -->
    <project path="app" name="wjdready/vim" remote="github" />

</manifest>
```


#### 2. 使用 repo 拉取清单中的项目

```sh
# 首先获取 repo 脚本, 可放到某位置并导出到 PATH
curl https://mirrors.tuna.tsinghua.edu.cn/git/git-repo -o repo

# 设置 repo 脚本的仓库镜像, 因为执行 repo 还需要下载一堆依赖和源码
export REPO_URL=https://mirrors.tuna.tsinghua.edu.cn/git/git-repo

# 方法1. 使用远程的清单项目, -b 可以指定分支, -m 指定清单文件
repo init -u https://github.com/yourname/myproject-manifest.git -b master -m default.xml 

# 方法2. 使用本地路径初始化, 同样需要 commit 才行
# 因为 manifests 项目会被 clone 到 .repo/manifests 并 reset 到 ^HEAD
repo init -u local/path/to/myproject-manifest -b master -m default.xml
```

#### 3. 版本确定规则

默认继承：当子项目不指定 revision 时，会继承 <default> 标签中定义的版本

1. 创建版本快照

```sh
# 1. 生成包含当前所有项目版本的清单文件
repo manifest -r -o .repo/manifests/release-v1.0.0.xml

# 2. 提交并推送清单文件
cd .repo/manifests
git add release-v1.0.0.xml
git commit -m "Release v1.0.0"
git tag v1.0.0
git push origin v1.0.0

# 使用特定版本的清单文件初始化
repo init -u https://github.com/yourname/myproject-manifest.git -b v1.0.0 -m release-v1.0.0.xml
repo sync
```

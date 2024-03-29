---
mytags: myblog
title: Git 常用操作
date: 2023-02-15 17:02:15
categories: [Git]
tags: [Git]
---

本文主要介绍 Git 常用操作
<!-- more -->

# Git 常用操作

### git clone 和 git remote 
```sh
git remote add origin xxx.git

# 添加另一个远程仓库
git remote add gitee_origin xxx2.git

# 查看远程仓库情况
git remote -v

# 推送
git push -u origin master 
git push -u gitee_origin master

# 拉取
git pull origin master 
```

### git status


```shell
# 查看忽略的文件或文件夹有哪些
git status --ignored
```

### git add

```shell
# [+--] 强制添加某个文件或文件夹, 忽略 .gitignore 中的设置 
git add --force protected/ext/SpecificFolder

```

### git rm

```shell

# [+++] 删除某次 add, 比如不小心 add 某个文件 又想取消掉
git rm --cached xxx

```

### git diff 

```shell

# [+++] 查看未已经更改但是未 add 的内容
git diff

# [+++] 查看已经 add 到缓存的内容
git diff --cached

```

### git log

log 可以显示有哪些文件更改了, 更改了多少, 但是没没法看具体的更改内容

```shell
# [+++] 查看提交信息
git log

# [+++] 提交信息并显示具体文件修改记录
git log --stat

# [++-] 以表格形式显示
git log --stat --graph
```

# git show

show 可用来显示具体的文件更改信息, 具体到每一行

```shell
# 默认显示 HEAD 的更改信息
git show

# 查看某次 commit 的更改内容
git show <commit>

```

### git submodule

submodule 允许外部存储库嵌入源代码树的专用子目录中

```sh
# 将每个子项目添加到主项目
git submodule add <url>

# 将项目添加到您的存储库后，您必须初始化并更新它
git submodule init
git submodule update
```


### 本地仓库

```shell
# 可以克隆自本地的某个仓库, 比如
git clone C:/Users/shino/Desktop/gogoanime

# 也可以将远程仓库设置为本地某个地方
git remote add origin C:/Users/shino/Desktop/gogoanime
# 查看当前远程仓库信息
git remote -v

# 然后就可以 pull , push
git pull origin master  --allow-unrelated-histories
```

### git clone

```sh
# 克隆深度
git clone --depth [depth] [remote-url]

```

#### git filter-branch

```sh
# 删除不小心提交的文件
git filter-branch --tree-filter 'rm -rf path/to/spurious/asset/folder' [commit]
```


# 常见问题

### HOME 目录更改

在提交时 git 会在当前用户的 HOME 目录下查找 .ssh 文件夹, 根据里面的内容来获取公钥和密钥，因此若出现

```
no matching host key type found. Their offer: ssh-rsa,ssh-dss
```

有可能是安装其他软件时, HOME 目录被篡改成其他目录，导致 git 无法获取 .ssh 文件夹了

### 合并另一个分支中的某个文件

很多时候在另一个分支更改了, 但是不想合并全部, 可指定合并某个文件

```
git checkout --patch 来源分支名称 文件路径
```

### 拉取远程分支

```
git checkout -b develop origin/develop
```

### 暂存代码

有时有突发工作，这时我们可先暂时存储当前代码, 转去做其他内容

```sh
# 暂存命令
git stash save "暂存的备注"

# 查看暂存记录
git stash list

# 取回暂存代码
git stash pop

# 取回代码后删除某个缓存
git stash drop stash@{0}

# 清空全部缓存
git stash clear 

# 取消【取消暂存代码】 (慎用)
git reset --hard

# git stash 冲突
https://blog.csdn.net/jy692405180/article/details/78520251
```


### 对某个 commit 打标签

git tag -a v0.1.1 9fbc3d0

### 对于 Windows 下换行符自动转换

[参考](https://blog.csdn.net/Babylonxun/article/details/126598477)


### 修改之前提交的用户信息

[参考](https://zhuanlan.zhihu.com/p/455741996)


### 重新提交某个版本

有时候提交版本有问题，想做细微更改后重新提交；使用 commit --amend 就可以做到

回退到刚提交的版本, 修改后执行:

```sh
git commit --amend
```

## 撤销最新提交并放回到缓存中

```sh
git reset HEAD^
```


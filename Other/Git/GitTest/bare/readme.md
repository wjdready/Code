---
mytags: myblog
title: Git bare 仓库测试
date: 2023-02-15 17:44:56
categories: [Git]
tags: [Git]
---

本文主要介绍如何使用 Git 的 bare 仓库来实现 Git 仓库包含多个子仓库

<!-- more -->


# Git bare 仓库测试

我们假设有一个主项目 MainProject, 其下有多个子项目 SubProject1, SubProject2

我们希望每个子项目中都有自己的 git 管理仓库, 但是又希望所有代码都放在 MainProject 目录中, 

MainProject 是关联到远程仓库的, 我们希望 push 或 pull 的时候, 把子项目也加载一起, 

如果单纯的在子项目中使用 `git init` 是行不通的, 因为主项目在提交 commit 时会忽略包含 .git 的文件

那该怎么办呢, 一般来说, 可以使用 submodule 的方式来实现一个项目中包含多个 git 仓库, 但是前提是要将那些子模块仓库放在其他远程仓库,

这样就比较麻烦, 因为如果子项目很多, 就要创建非常多的仓库来容纳他们, 但其实我希望的是所有代码都统一在主项目中, 而把他们分成多个子项目只是希望能对子项目单独进行 git 管理. 

其实 git 的 bare 功能能够解决上面的问题, 下面看看如何实现:

```shell 

# 0. 创建主项目目录
mkdir MainProject -p && cd MainProject

# 初始化主项目
git init .
echo Hello > hello.txt
git add . && git commit -m "Add hello.txt"
git remote add origin https://gitee.com/wjundong/test-git.git

# 1. 子项目1
mkdir SubProject1 -p && cd SubProject1

# 创建子项目的 bare 仓库
mkdir .git_bare -p && cd .git_bare
    git init --bare .
cd ..

# 子项目的 git 管理仓库
mkdir subproject -p && cd subproject
    git init .
    echo "SubProject1" > SubProject1.txt
    git add . && git commit -m "Add SubProject1.txt"

    git remote add bare ../.git_bare
    git push bare master
cd ..

# 主项目中添加 bare
git add . && git commit -m "add SubProject1"
git push origin master

### X. 后面某个时间, 我们从远程仓库下载我们的主项目 (包含了子项目)
mkdir TEST -p && cd TEST
git clone https://gitee.com/wjundong/test-git.git .
git log

# 将 subproject 从 bare 中解析出来
cd SubProject1
git clone .git_bare subproject

cd subproject
git log
echo "Very Good" > good.txt
git add . && git commit -m "add good.txt"
git push
```

.git_bare 相当于一个本地的仓库服务器, 它可以被主项目添加到源代码中

> 注: 如果在已经存在的仓库文件夹创建 bare 发现即使该文件夹中没有 .git 也无法添加该文件夹,
> 可以使用 git rm --cached thisdir 来重新添加

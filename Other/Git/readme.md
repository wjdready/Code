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

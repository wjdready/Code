
# Git 常用操作

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

* 子模块

下载的源码中包含有另一个模块

```sh
# 初始化模块依赖
git submodule init

# 更新模块信息
git submodule update
```
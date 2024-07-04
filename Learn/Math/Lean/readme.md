

官方地址:
https://github.com/leanprover/lean4

文档:
https://lean-lang.org/documentation/

从游戏中入门 lean4:
https://adam.math.hhu.de/#/

在线刷题:
https://www.codewars.com/


## 安装
从官方git仓库中下载最新版本, 解压后设置环境变量 

当然也可以使用  Lean 的版本管理器 [elan](https://github.com/leanprover/elan) 进行安装:

```sh
curl https://raw.githubusercontent.com/leanprover/elan/master/elan-init.sh -sSf | sh
elan self update
elan default leanprover/lean4:stable
```

## 创建项目

```sh
mkdir test && cd test
lake init foo
lake build
# 得到 foo 可执行文件
./.lake/build/bin/foo
```


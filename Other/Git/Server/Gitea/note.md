

从官网下载最新: https://dl.gitea.com/gitea/

将 exe 放到 D:\Documents\gitea 运行 exe, 然后在网页端运行并配置即可

设置成自带 SQLite 数据库，就不需要额外安装数据库了。其他默认即可, exe 和 数据配置文件都在 gitea 目录下, 方便管理

当然这些都可以在安装后 app.ini 中配置


一些配置:

```ini
; 配置官方文档 https://docs.gitea.com/zh-cn/next/administration/config-cheat-sheet

RUN_USER = git                      ; clone ssh 时 git@ 开头

[repository]
ROOT = D:/Documents/gitea/data/gitea-repositories
ENABLE_PUSH_CREATE_USER = true      ; 是否允许用户推送时自动创建仓库
ENABLE_PUSH_CREATE_ORG = true       ; 是否允许组织推送时自动创建仓库
DEFAULT_PUSH_CREATE_PRIVATE = true  ; 默认创建的仓库是否为私有

[server]
START_SSH_SERVER = true             ; 是否启动SSH服务器
```

# tea 工具

工具地址: https://gitea.com/gitea/tea

tea 工具可以方便的管理仓库, 例如创建仓库, 转移仓库到组织等

```sh
# 登陆
tea login add
tea login default localhost:3000

# 列出所有仓库
tea repo

# 移动仓库到组织
# UI 中打开仓库->设置->滑到最下面->转移仓库所有权
```

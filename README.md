<!-- myblog -->

# 个人代码管理

## Make 系统

某个目录如果存在源码并且需要运行，makefile 模板如下

```makefile
# [MyProject]

default:
	g++ main.c

run:
	./a.exe

clean:
	@rm ./a.exe 2> /dev/null || true 

globalclean:
	make clean 

fullclean:
	make clean

```

第一行的 `# [MyProject]` 指明模板类型, 如果该行不存在，扫描脚本将忽略该 Makefile

* MyProject 模板须包含 `run`, `clean`, `globalclean` 目标, 
  其中 `globalclean` 目标是顶层目录 Makefile 在清除不必要文件时调用的,
  `clean` 则可用于本地调试代码时清除临时数据用的, `fullclean` 用于完全清理， 他会对所有临时文件或中间不必要文件进行删除,
  这可能会删除一些下载的包, 因此谨慎使用.

## 博客自动生成系统

依赖安装

```sh
npm install -g hexo-cli
# 下载最新版的 pandoc 并导出到环境变量
https://pandoc.org/installing.html
```

执行 `make blog` 可自动生成博客网站, `make blog_server` 在本地查看, `make blog_deploy` 则发布更新的博客网站，
执行 `make blog` 之后会自动扫描目录下所有的 Markdown 并输出到博客网站, 为了格式的统一, 每篇 Markdown 都应遵循如下格式

```markdown
---
mytags: myblog
title:
date: 2023-01-15 23:22:48
categories:
tags:
---

文章内容...

```

其中 `mytags: myblog` 必须存在并放在第二行, 否则扫描脚本会跳过该 Markdown 文件, 可执行 make blog_template 来输出模板.

对于根目录下的文件夹, 如果是 `.` 或 `_` 开头的文件夹, 扫描脚本会进行忽略，请不要将要发布的 Markdown 文档放到这些文件夹下, 比如 .vscode 文件夹.


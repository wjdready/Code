
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

fullclean:
	make clean 
```

第一行的 `# [MyProject]` 指明模板类型

* MyProject 模板须包含 `run`, `clean`, `fullclean` 目标, 
  其中 `fullclean` 目标是顶层目录 Makefile 在清除所有不必要文件时调用的,
  `clean` 则可用于本地调试代码时清除临时数据用的



[参考](https://stackoverflow.com/questions/9036859/what-are-the-exact-steps-for-creating-and-then-linking-against-a-win32-dll-on-th)

任何时候都可以通过 `cl -help` 来获得帮助. 当然最重要的是[官方文档](https://learn.microsoft.com/zh-cn/cpp/build/reference/output-file-f-options?view=msvc-170)

关于动态库的几个比较重要的点:

* 要导出动态库, 源文件对需要导出的函数使用 `__declspec(dllexport)` 修饰.
* cl -c 表示仅编译不链接
* cl -link 表示进行链接
* -out:out/path 是链接后输出的目录
* cl -LD 表示创建动态库
* -implib:out/path 是指定输出动态库的路径, 这一点是通过观察编译步骤发现的, 发现编译是会给出一个默认的输出路径，比如 /out:add.dll 和 `/implib:add.lib`.
  如果不指定，则使用这些默认的，这就意味着我们可以通过 -xxx:out/path 指定输出路径 (注意，要有这个提示，需要去掉 -nologo选项)
* lib 用来创建静态库
* 发现编译链接命令行参数顺序很重要，比如 
```sh
# libpath 识别不出来
$(CC) $(OBJS) -libpath:lib -link $(LIBS) -out:$(BUILD_DIR)/$(PROJECT_NAME).exe 

# 需要放在 -link 后面
$(CC) $(OBJS) -link -libpath:lib $(LIBS) -out:$(BUILD_DIR)/$(PROJECT_NAME).exe 
```



[官方教程](https://learn.microsoft.com/zh-cn/cpp/build/projects-and-build-systems-cpp?view=msvc-170)

## MSYS2 上导出 VC 环境的尝试

Google: using cl.exe in msys2
https://anadoxin.org/blog/bringing-visual-studio-compiler-into-msys2-environment.html/

最主要的难点在于官方提供的导出环境的脚本，例如 vcvars64.bat 无法直接在 MSYS2 中导出，感觉是因为其使用 call 的缘故，而 msys2 执行 batch 时使用的是新进程，变量是隔离的，运行完后导出的变量就被无效了。而 cmd 直接运行 vcvars64.bat 变量是存在的。

最后只能采用比较折衷的方法，那就是新建一个 msysvc.bat

```bat
call vcvars64.bat & msys.bat
```

先运行 vcvars64.bat 再运行 msys, 这样 msys 将继承 vcvars64.bat 导出的环境，从而 msys 拥有 VC++ 的环境. 

该脚本可在 VSCode 用于通过拆分终端来执行并进入，也可直接在 cmd 中执行进入


## link 参数

https://learn.microsoft.com/en-us/cpp/build/reference/linker-options?view=msvc-170


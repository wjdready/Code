
Bash Shell 真的是很丑陋，很难读，特别是是各种变量的命名，各种全局变量的定义和相互引用，看别人上千行的 bash 真的是头大，这时才想到能不能有个 Debug 可以单步运行之类的。

默认 bash 是有 debug 功能的, 

set -v -x -e 

另外 bashdb 是一个不错的工具, 这里给一个[参考](https://blog.csdn.net/qq_44643189/article/details/125242303)

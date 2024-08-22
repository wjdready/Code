

参考:
https://stackoverflow.com/questions/3808053/how-to-get-a-pointer-to-a-binary-section-in-msvc
https://learn.microsoft.com/en-us/cpp/preprocessor/init-seg?view=msvc-170&redirectedfrom=MSDN
https://devblogs.microsoft.com/oldnewthing/20181108-00/?p=100165


直接 cl 编译出来是 main 是 OK 的，但是使用 vs 编译会被优化，导致 static 定义时被删除。

更改 vs 设置，属性->C/C++->优化，优化禁止掉

另外的解决办法是在 static 下面又定义一个指针，使得上面的 static 定义的不会被优化。但是这种情况当开启全程序优化后还是会被优化掉。

debug 版本总是会多出来一些东西


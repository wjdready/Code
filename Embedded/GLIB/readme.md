
## GLIBC 版本问题

通常需要在另一台机器编译, 但如果使用默认工具链则可能会导致 GLIBC 版本不兼容

```sh
# 查看当前系统的 GLIB 版本
ldd --version 

```

一段简单的依赖 GLIBC 库的程序, 用于验证是否可在目标平台执行

```c
#include <stdio.h>
#include <gnu/libc-version.h>

int main() 
{
    printf("GLIBC版本: %s\n", gnu_get_libc_version());
    return 0;
}
```

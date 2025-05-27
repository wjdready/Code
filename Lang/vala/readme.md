

## 调用 C 库

```c
// example.c
int add(int a, int b) 
{
    return a + b;
}

// example.h
int add(int a, int b);

// build
// gcc -shared -o libexample.so example.c
// gcc -shared -o example.dll example.c
```

```c#
// main.vala
[CCode (cheader_filename = "example.h")]
public extern int add(int a, int b);

public class Main {
    public static int main(string[] args) {
        int result = add(3, 5);
        stdout.printf("3 + 5 = %d\n", result);
        return 0;
    }
}

// build
// 插入 gcc 编译标志用 -X
// --verbose 查看详情编译信息
// valac --pkg gobject-2.0 main.vala -X -I/c/Users/shino/Desktop/dev/vala -X -L/c/Users/shino/Desktop/dev/vala -X  -lexample --verbose
```

## C 调用 Vala

```c#
// valafun.vala

[CCode (cname = "vala_max")]
public int max(int a, int b) 
{
    return a > b ? a : b;
}

// build
// valac --pkg gobject-2.0 --library=valafun valafun.vala -X -shared -X -fPIC -o valafun.dll
```

```c
// test.c
#include <stdio.h> 

extern int vala_max(int a, int b);

int main(int argc, char const *argv[])
{
    printf("vala_max = %d\n", vala_max(2, 4));
    return 0;
}

// build
// gcc test.c -L. -lvalafun -lgobject-2.0 
```



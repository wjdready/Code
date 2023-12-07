
libfdt 可通过 `pacman -S mingw-w64-ucrt-x86_64-dtc` 安装，但是从内核的 `scripts/dtc/libfdt` 中直接提取也是可以直接使用的。因为 `libfdt` 不依赖任何 IO 相关的内容。

## 基本

```c
#include <libfdt.h>
#include <stdio.h>
#include <stdlib.h>

void *load_from_file(const char *filepath)
{
    FILE *fp = fopen(filepath, "rb");
    if (fp == NULL)
    {
        printf("Failed to open device tree file\n");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long fdt_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    void *fdt = malloc(fdt_size);
    fread(fdt, 1, fdt_size, fp);
    fclose(fp);

    return fdt;
}

int main(void)
{
    void *fdt = load_from_file("demo.dtb");
    if(fdt == NULL)
        return -1;

    if (fdt_check_header(fdt) != 0)
    {
        printf("\nERROR: Invalid device tree blob\n");
        return -1;
    }

    free(fdt);
    return 0;
}
```

## 获取头信息

为了可移植性，应当使用 `fdt_get_header` 宏可以获取头信息，其会将头的小端字节序自动转为适合主机字节序。

如果直接使用 `struct fdt_header *head = (struct fdt_header *)fdt;` 去访问头，那么在大端主机上将为反的字节序。

```c
fdt32_t magic = fdt_get_header(fdt, magic);
fdt32_t version = fdt_get_header(fdt, version);
printf("%0X %0X\n", magic, version);
```

## 根据路径获取一个节点的偏移地址

`fdt_path_offset(fdt, fullpath)` 可根据提供的节点完整路径，返回节点的偏移地址

拿到节点的偏移地址后就可以调用很多函数，做很多事情了。比如获取节点名 `fdt_get_name`

例如有一个 

```lua
/dts-v1/;
/ {
    A {
        company = "FFF";
        B {
            my = "aaa";
        };
    };
};
```

```c
int nodeoffset = fdt_path_offset(fdt, "/A/B");
const char *name = fdt_get_name(fdt, nodeoffset, NULL);
printf("node-name: %s\n", name); // node-name: B

// 反过来，也可以通过给定一个节点偏移地址来获取该节点的完整路径
char buf[256];
fdt_get_path(fdt, nodeoffset, buf, sizeof(buf));
printf("%s", buf);              // A/B
```


> 对于根路径 '/', 其节点偏移地址总是 0

## 遍历节点属性和子节点

`fdt_for_each_property_offset` 宏可用于遍历某个节点下的所有属性，然后在每一次迭代中，再通过 `fdt_getprop_by_offset` 获取具体的属性名和属性值。

`fdt_for_each_subnode` 可用于遍历某个节点下的所有子节点，再通过 `fdt_get_name` 获取子节点的节点名。

例如实现一个递归打印指定节点的函数 `print_node`

```c
#define MAX_DEPTH 32
static char depth_set[MAX_DEPTH];

static void pretty_node(int depth)
{
    if (depth == 0)
        return;

    for (int i = 0; i < depth - 1; ++i)
        printf(depth_set[i] ? "│   " : "    ");

    printf(depth_set[depth - 1] ? "├── " : "└── ");
}

static void pretty_prop(int depth)
{
    for (int i = 0; i < depth; ++i)
        printf(depth_set[i] ? "│   " : "    ");

    printf(depth_set[depth] ? "│  " : "   ");
}

static void print_node_prop(const void *fdt, int node, int depth)
{
    int prop;
    fdt_for_each_property_offset(prop, fdt, node)
    {
        if(depth >= 0)
            pretty_prop(depth);
        int size;
        const char *name;
        const char *value = fdt_getprop_by_offset(fdt, prop, &name, &size);

        bool is_str = !(size > 1 && value[0] == 0);
        if (is_str)
        {
            // Scan through value to see if printable
            for (int i = 0; i < size; ++i)
            {
                char c = value[i];
                if (i == size - 1)
                {
                    // Make sure null terminate
                    is_str = c == '\0';
                }
                else if ((c > 0 && c < 32) || c >= 127)
                {
                    is_str = false;
                    break;
                }
            }
        }

        if (is_str)
        {
            printf("[%s]: [%s]\n", name, value);
        }
        else
        {
            // printf("[%s]: <bytes>(%d)\n", name, size);
            printf("[%s]: <bytes>(%d) ", name, size);
            for (int i = 0; i < size; i++)
                printf("0x%02X ", value[i]);
            printf("\n");
        }
    }
}

static void print_node(const void *fdt, int node, int depth)
{
    // Print node itself
    pretty_node(depth);
    printf("#%d: %s\n", node, fdt_get_name(fdt, node, NULL));

    // Print properties
    depth_set[depth] = fdt_first_subnode(fdt, node) >= 0;
    print_node_prop(fdt, node, depth);

    // Recursive
    if (depth_set[depth])
    {
        int child;
        int prev = -1;
        fdt_for_each_subnode(child, fdt, node)
        {
            if (prev >= 0)
                print_node(fdt, prev, depth + 1);
            prev = child;
        }
        depth_set[depth] = false;
        print_node(fdt, prev, depth + 1);
    }
}
```

测试

```c
// 由于根节点的偏移总是 0, 传入 0 节点直接打印整棵树
print_node(fdt, 0, 0);

// 或者只打印该节点的所有属性
print_node_prop(fdt, 0, -1);
```


## phandle 

phandle 属性是设备树编译时自动生成的，用于节点之间的相互引用。只有当一个节点被引用时才会生成该节点的 phandle.

通过 phandle 可以访问到被引用节点的偏移地址，例如

demo.dts

```lua
/dts-v1/;

/ {
    A {
        company = "FFF";
        B {
            my = "aaa";
            dd = <&d0>;
        };
    };

    C {
        d0: D {
            my = "DD";
        };
    };
};
```

mian.c

```c
int phandle_max = fdt_get_max_phandle(fdt);
for (int i = 1; i <= phandle_max; i++)
{
    int node = fdt_node_offset_by_phandle(fdt, i);
    print_node_prop(fdt, node, -1);
}
```


## 一些通用函数

设备树标准中指定了一些标准属性，libfdt 提供了一些函数方便我们获取这些标准属性。

`#address-cells` 用于指定子节点中 reg 属性的**地址部分**需要几个单元格来描述，默认为 2
`#size-cells` 用于指定子节点中 reg 属性的**大小部分**需要几个单元格来描述，默认为 1

```c
int addr_cells = fdt_address_cells(fdt, 0);
int size_cells = fdt_size_cells(fdt, 0);
printf("%d %d", addr_cells, size_cells);
```

## 增删改查

一般来说，将设备树加载到内存后系统对其将总是只读的。但是再系统启动之前，比如 u-boot 却可以通过对加载的设备树进行进一步修改，之后再传给系统。

至于系统能否在运行时通过修改设备树，使其动态生效这一主题还有待探索。

要对读入的 dtb 文件进行修改，首先应当加载设备树到更大的内存区域，这一点通过 `fdt_open_into` 实现.

下面将对 `phandle` 小节用到的 `demo.dts` 进行进一步操作

```c
int fdt_size = fdt_totalsize(fdt);
void *buf = malloc(fdt_size + 100);
fdt_open_into(fdt, buf, fdt_size + 100);
free(fdt);
fdt = buf;

int node = fdt_path_offset(fdt, "/A/B");

// 增
int res = fdt_add_subnode(fdt, node, "gogo");
if(res > 0)
    fdt_setprop_string(fdt, res, "ff", "soso");

// 改
fdt_setprop_string(fdt, node, "company", "MMM");
print_node(fdt, 0, 0);

// 删
fdt_del_node(fdt, node);
print_node(fdt, node, 0);
```

## 覆盖节点 overlay

参考文档:
https://docs.u-boot.org/en/latest/usage/fdt_overlays.html
https://www.kernel.org/doc/Documentation/devicetree/overlay-notes.txt

overlay 的两者写法

```lua
/dts-v1/;
/plugin/;

 / {
    fragment@0 {
        target-path = "/your/node/path";
        __overlay__ {
            /* 在此添加要插入的节点属性或子节点 */
        };
    };

    fragment@1 {
        target = <&node_alias_name>;
        __overlay__ {
            /* 在此添加要插入的节点属性或子节点 */
        };
    };
 };
```

```lua
/dts-v1/;
/plugin/;

&{/your/node/path} {
    /* 在此添加要插入的节点属性或子节点 */
};

&node_alias_name {
    /* 在此添加要插入的节点属性或子节点 */
};
```

例如现有一个 demo_overlay.dts

```lua
/dts-v1/;
/plugin/;

/ {
    fragment@0 {
        // target-path = "/C/D";
        target = <&d0>;
        __overlay__ {
            status = "okay";
        };
    };
};

// &{/C/D} {
// &d0 {
//     status = "okay";
// };
```

当使用 `<&d0>` 的方法指定覆盖节点时，在编译设备树的时候需要加入 `-@` 选项, 如果是使用的是完整路径的方法，则不需要。这是因为 `-@` 选项在编译时将生成一个 `__symbols__` 节点，用于解决使用 `<&d0>` 方法时的路径引用问题。即:

```
dtc -@ -I dts -O dtb demo.dts -o demo.dtb
dtc -@ -I dts -O dtb demo_overlay.dts -o demo_overlay.dtb
```

使用 `fdt_overlay_apply` 函数对设备树进行覆盖:

```c
void *fdt_overlay = load_from_file("demo_overlay.dtb");
if(fdt_overlay == NULL)
    return -1;

print_node(fdt, 0, 0);

int fdt_size = fdt_totalsize(fdt);
void *buf = malloc(fdt_size + 100);
fdt_open_into(fdt, buf, fdt_size + 100);
free(fdt);
fdt = buf;

int ret = fdt_overlay_apply(fdt, fdt_overlay);
if(ret == 0)
    print_node(fdt, 0, 0);
else
    fprintf(stderr, "\nFailed to apply %s\n", fdt_strerror(ret));
```

> 重新分配大小是需要的，因为要涉及设备树的增删改

为了有更好的兼容性，更加通用的方法是使用 realloc 来动态更改分配内存的大小:

```c
static void *apply_one(char *base, const char *overlay, size_t *buf_len, const char *name)
{
    char *tmp = NULL;
    char *tmpo;
    int ret;

    /*
     * We take a copies first, because a a failed apply can trash
     * both the base blob and the overlay
     */
    tmpo = malloc(fdt_totalsize(overlay));

    do
    {
        tmp = realloc(tmp, *buf_len);
        ret = fdt_open_into(base, tmp, *buf_len);
        if (ret)
        {
            fprintf(stderr, "\nFailed to make temporary copy: %s\n", fdt_strerror(ret));
            goto fail;
        }

        memcpy(tmpo, overlay, fdt_totalsize(overlay));

        ret = fdt_overlay_apply(tmp, tmpo);
        printf("res %d\n", ret);

        if (ret == -FDT_ERR_NOSPACE)
        {
            *buf_len += 128;
        }
    } while (ret == -FDT_ERR_NOSPACE);

    if (ret)
    {
        fprintf(stderr, "\nFailed to apply '%s': %s\n", name, fdt_strerror(ret));
        goto fail;
    }

    free(base);
    free(tmpo);
    return tmp;

fail:
    free(tmpo);
    if (tmp)
        free(tmp);

    return NULL;
}
```

然后使用该函数

```c
void *fdt_overlay = load_from_file("demo_overlay.dtb");
if(fdt_overlay == NULL)
    return -1;

print_node(fdt, 0, 0);

size_t len = 512;
void *newfdt = apply_one(fdt, fdt_overlay, &len, "demo_overlay.dtb");
if(newfdt)
    print_node(newfdt, 0, 0);
```


## dts 中的 include

在 dts 中使用 `#include` 时需要使用 `cpp` 事先进行预处理，可先输出中间文件再使用 dtc 进行编译

事实上，cpp 的这种处理方法并不是专门为 dts 设置的，对于任何文件都可以使用类似的 `#include` 方法引入头文件，然后通过 cpp 进行预处理输出宏替换后的文本，再进行进一步处理。

设 gpio.h 中有定义 `#define GPIO_ACTIVE_HIGH 0`, 现有如下 demo.dts 

```lua
/dts-v1/;
#include <base.dtsi>

/ {
    flag = <GPIO_ACTIVE_HIGH>;
    foo = "foo";
};
```

其中 base.dtsi 为

```lua
#include <gpio.h>

/ {
    foo = "goo";
};
```

编译

```sh
cpp -nostdinc -I. -undef -x assembler-with-cpp demo.dts -o demo.dts.tmp
dtc -I dts -O dtb demo.dts.tmp -o demo.dtb
```

宏替换后将得到 `demo.dts.tmp` 如下

```lua
/dts-v1/;

/ {
    foo = "goo";
};

/ {
    flag = <0>;
    foo = "foo";
};
```

> 最终 foo 的值为 "foo", 可见当出现同名属性时，新的定义将覆盖前面的定义

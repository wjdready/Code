


# 介绍

这是一个个人脚本工具管理系统, 支持 python 和 bash shell 

使用前需将环境变量导出到 PATH, 当然可以在 ~/.bashrc 中添加, 这样每次打开中断都自动导出环境了.

```shell
export PATH=$PATH:~/.myscripts/shell:~/.myscripts/python
```

### 脚本编写规范:

每个脚本都必须能够处理给定参数 -h 并显示描述和使用方法, 例如

对于 bash

```shell
if [ "$1" == "-h" ]; then
    echo "Description:"
    echo "    Find all dll dependencies of an executable file under a given [path]."
    echo "    If no [path] parameter is given, the current call path is used."
    echo "Usage: "
    echo "    $(basename $0) [path]"
    return
fi
```

对于 python

```python
if len(sys.argv) != 3:
    print("Description:\n    C language output function declarations and comments to header files")
    print("Usage:\n    %s <source_file> <header_file>" % os.path.basename(sys.argv[0]))
    exit(0)
```

这里 python 虽然没有显式处理给定 -h 参数, 但只要对给定的 -h 参数能够返回帮助信息即可.

脚本的必选参数用 `<arg>` 描述, 可选参数用 `[arg]` 描述. 另外为了能够在命令行中自动补全, 脚本的最开头应该给出解释器名称, 例如:

对于 bash 

```shell
#!bash
```

对于python

```shell
#!python3
```

### 用法

在 python 或 shell 文件夹中新建一个脚本, 完成后执行 `make apply` 即可, 新加入的脚本将更新到 `~/.myscripts` 中, 然后就可以直接在命令行中使用脚本了. 

所有个人的脚本都以 `my` 开头, 在执行 `make apply` 后, 本地的脚本文件在复制到  `~/.myscripts` 的时候将文件名添加了前缀 `myshell` 或 `mypython`. 因此运行时要在脚本文件名的前面加这些前缀才能运行.

另外, 提供了一个 `myshell_help.sh` 的脚本, 可以列出当前支持的所有脚本和用法, 可通过 `myshell_help.sh -h` 来查看 help 的用法.


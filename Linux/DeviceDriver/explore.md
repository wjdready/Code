

# [2023-11-29 14:52:36]

##### 在系统上查看设备

设备树中描述的设备在解析后会导出到 /sys/devices 目录, 里面包含了各种设备，按文件树形式管理。

当设备驱动被加载时, 会先向内核申请一个设备号，例如:

```c
/* 传入 0, 代表由操作系统自动分配主设备号 */
major = register_chrdev(0, "hello", &drv_hello);
```

然后根据需求来决定是否创建设备节点:

```c
/* 注册驱动后使用 class_create  device_create 创建设备节点 */
hello_class = class_create(THIS_MODULE, "hello_class");
device_create(hello_class, NULL, MKDEV(major, 0), NULL,"hello");
```

通过 device_create 函数创建设备节点，设备节点被组织到 /sys/dev, 但是为了方便归类，一般还会使用 class_create 来创建一个分类

然后我们将可以在 /sys/class 中查看分类. 

因此 /sys/devices 只是列出了拥有哪些硬件设备，但真正加载了驱动的设备，正常使用的设备才被添加到 /sys/class

```sh
# 列出所有定义的平台设备，目录名称就是在设备树中定义的节点名, 比如 alias_name: node_name { }
# 别名 alias_name 并不会展示出来哦, 后面的 node_name 才是真正的节点名
ls /sys/devices/platform

# linux 设备树加载进来后，默认状态为 disable ,如何在启动后动态使能一个设备
```

##### ALSA

alsa-utils 工具

提供3个有用的命令: arecord

```sh
amixer --help
```

#### 查看某个驱动模块是否被编译成功

有时执行 make menuconfig 后已经设置好了，但是使用的是某些官方提供的内核，可能会覆盖 .config ，也就说说，刚设置的内核无效了。

一般来说只要查看内核文件夹下指定内核源文件被编译成了 .o 文件就可以确定该模块确实被编译成功了


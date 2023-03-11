#### 02 使用 newlib 堆

[参考](https://llvm-gcc-renesas.com/wiki/index.php?title=How_does_Newlib_initialize_the_heap%3F)


# 01 Map 文件

[参考](https://interrupt.memfault.com/blog/get-the-most-out-of-the-linker-map-file)


## Glibc 中的特殊的段
 

| 段名  | 备注                                                            |
| ----- | --------------------------------------------------------------- |
| .init | 一个函数放到.init段，在main函数执行前系统就会执行它             |
| .fini | 假如一个函数放到.fini段，在main函数返回后该函数就会被执行       |

## __libc_init_array 这个函数中执行的关键过程如下：

调用 .preinit_array 段中的预初始化函数
调用 .init 段中的 _init 函数
调用 .init_array 中的所有函数

这里我需要解释下，在一个段中可能会存在多个函数，根据链接脚本的写法，链接器会在链接时将段名相同的函数指针放到同一个段中，然后通过在段的前后设定锚点来依次执行相应的函数，这样的方式在一些系统的驱动初始化中也能见到！

与 __libc_init_array 对应的函数是 __libc_fini_array，这个函数是在 main 函数执行完成后执行的。它首先会调用 .fini_array 中的所有函数，然后调用 _fini 函数，在嵌入式中一般不会执行到 __libc_fini_array 函数。

在 pc 端也有类似的过程，在我的 debian 10 中 _init 与 _fini 函数在 crti.o 中被定义，详细的信息如下
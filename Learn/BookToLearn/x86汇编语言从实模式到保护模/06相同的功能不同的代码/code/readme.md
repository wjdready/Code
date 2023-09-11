


事实上汇编就能产生可执行的代码了

```sh
as start.s -o start.o                   # 这将产生 elf 格式的目标文件, 文件中的标签偏移地址都是基于 0 的
objcopy -O binary start.o start.bin     # 生产 bin 文件可执行
```

[关于 org 0x7c00 ](https://www.jianshu.com/p/dbbbc714f942)


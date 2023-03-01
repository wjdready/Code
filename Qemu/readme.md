

### 基本要点

这个[教程](https://documentation.suse.com/sles/15-SP2/html/SLES-all/cha-qemu-running.html)挺全的

##### 1. 参数规则

参数通用格式

```
-arg option=content,option1=content,option2=content
```

除了 -arg 外后面的每一项都有可能省略

比如

* `-nographic` 参数没有附加选项
* `-device usb-host,hostbus=0,hostaddr=4` -device 参数后有必选项, 指出设备类型 usb-host, 后面还2个跟附加选项用来对该 usb-host 进行描述
* `-drive if=none,file=$LFS/dst/rootfs.img,id=hd0` -drive 参数后面有3个选项用来描述 drive 设备

##### 2. guest 和 host

官方文档经常出现的 guest 和 host 是什么呢, host 是指你用来跑 QEMU 的那个主机系统, guest 是运行在 QEMU 里面的那个系统.




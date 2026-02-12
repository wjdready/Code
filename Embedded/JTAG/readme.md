

openocd.cfg

```shell
# 命令行方式
# openocd -f interface/stlink.cfg -f target/stm32h7x.cfg  

# this need match your debug adaptor and target device
source [find interface/stlink.cfg]
source [find target/stm32h7x.cfg]
# gdb_port 5000

# 复位设置, 对于 ST-link SWD 模式, 必须设置才能复位成功
reset_config none separate

# 下载 hex 文件, 前提是上面执行顺利
program  build/H743Go.hex

# 其他, 未测试有效性
# connect_assert_rst 标志将强制OpenOCD在初始连接期间重置CPU，使其脱离睡眠模式并确保调试通信成功。
# reset_config trst_and_srst srst_nogate connect_assert_srst

# config openocd support OneOS
# $_TARGETNAME configure -rtos oneos

# halt target after gdb attached
# $_TARGETNAME configure -event gdb-attach { halt }

# 辅助操作  

# telnet
# telnet localhost 4444 
# > reset halt
# > flash write_image erase xxx.hex

# GDB
# arm-none-eabi-gdb xxx.elf 直接设置目标文件
# (gdb) file xxx.elf设置目标文件
# (gdb) target remote localhost:3333    连接服务器
# (gdb) target extended-remote localhost:3333 扩展连接服务器
# (gdb) monitor reset halt 发送复位和暂停命令
# (gdb) load 加载程序
# (gdb) c 运行
# (gdb) b main 设置断点
# (gdb) n 单步执行
```



参考文献

官方文档 [Reset-Configuration](https://openocd.org/doc/html/Reset-Configuration.html) 描述如何配置复位信号

官方文档[flash 指南](https://openocd.org/doc-release/html/Flash-Commands.html)


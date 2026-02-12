

## 使用方法

参考 
https://zhuanlan.zhihu.com/p/31684114575
https://blog.csdn.net/qazw9600/article/details/105457219


MingW 和 MSYS 环境解析 dmp 比较繁琐, MSYS 目前 dump_syms 有问题, 还无法使用. 

但自己也总结了一个比较简单的方法, 如下

```sh
# 出错后生成 dmp 执行时是能正常输出调用历史的, 但是是相对地址, 需要找出程序的基本地址
# 再通过 addr2line 转换为符号地址
minidump_stackwalk xxx.dmp > crash_report.txt

# 通过 nm 查找 main 函数的地址, 从而获得基准地址, 然后在基准地址的基础上 + 偏移量即可获得符号地址
nm xxx.exe | grep main

# 得到符号地址后, 再通过 addr2line 就能获取到符号地址对应的文件名和行号
addr2line -e xxx.exe 0x00401000
```


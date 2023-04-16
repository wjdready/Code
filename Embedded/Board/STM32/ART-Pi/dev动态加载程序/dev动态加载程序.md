

[参考](https://blog.csdn.net/sinat_31039061/article/details/109763932)

[官方文档](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/application-note/components/dlmodule/an0023-dlmodule?id=%e4%bd%bf%e7%94%a8-qemu-%e8%bf%90%e8%a1%8c%e5%8a%a8%e6%80%81%e6%a8%a1%e5%9d%97%e7%bb%84%e4%bb%b6)

```py
if PLATFORM == 'gcc':

    ...
    
    STRIP = PREFIX + 'strip'
    M_CFLAGS = CFLAGS + ' -mlong-calls -fPIC '
    M_CXXFLAGS = CXXFLAGS + ' -mlong-calls -fPIC'
    M_LFLAGS = DEVICE + CXXFLAGS + ' -Wl,--gc-sections,-z,max-page-size=0x4' +\
                                    ' -shared -fPIC -nostartfiles -nostdlib -static-libgcc'
    M_POST_ACTION = STRIP + ' -R .hash $TARGET\n' + SIZE + ' $TARGET \n'
```

执行

```sh
rtt.bat scons --target=ua -s

git clone https://github.com/RT-Thread/rtthread-apps.git

cd rtthread-apps
export RTT_ROOT=$PWD/../rt-thread
export BSP_ROOT=$PWD/../
rtt.bat scons --app=hello

file hello/hello.mo 
# hello/hello.mo: ELF 32-bit LSB shared object, ARM, EABI5 version 1 (SYSV), dynamically linked, stripped
# 可以看到 生成的 hello/hello.mo 是一个动态连接版本, 仅有 3K 大小
```

正如 [参考](https://blog.csdn.net/sinat_31039061/article/details/109763932) 所说的, 目前似乎不支持外部 flash 的动态库..



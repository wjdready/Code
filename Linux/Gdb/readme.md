
## 嵌入式 linux 调试

前提条件: 支持网络通信, 或使用 adb 的 usb 网络功能, 感觉是在不行串口也可以虚拟出网口出来

原理是在设备端执行 gdbserver :1234 ./a.out

VSCode 中实现按下 debug 时完成编译和上传 elf 文件

launch.json

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) 启动",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/a.out",
            "preLaunchTask": "Build",   // 开始前运行的 task
            "args": [],
            "stopAtEntry": false,       // 在进入debug 后暂停程序
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": true,    // 调试对象启动控制台
            "MIMode": "gdb",
            "miDebuggerPath": "C:/ProgramFiles/Library/arm-gnu-toolchain-13.3.rel1-mingw-w64-i686-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gdb.exe",
            "setupCommands": [
                {"text": "target extended-remote 192.168.1.125:1234"},
            ]
        }
    ]
}
```

tasks.json

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build",
            "type": "shell",
            // 编译并上传
            "command": "aarch64-none-linux-gnu-gcc.exe main.c -g && scp a.out orangepi@192.168.1.125:/home/orangepi",
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```



launch.json

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) 启动",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/H743Go.elf",
            "args": [],
            "stopAtEntry": true,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": true,
            "MIMode": "gdb",
            "miDebuggerPath": "arm-none-eabi-gdb",
            "setupCommands": [
                    {"text": "set remotetimeout 5"},
                    {"text": "target extended-remote localhost:3333"},
                    {"text": "monitor reset halt"},
                    // {"text": "load"},
                    {"text": "break main thread 1","ignoreFailures": true},
            ]
        }

    ]
}
```


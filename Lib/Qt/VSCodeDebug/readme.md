

# [2024-06-03 23:00:44]

#### 在 VSCode 上使用 msys2 debug 

无法使用 MSYS2 终端进行 launch, 需设置默认终端为 cmd 并且 launch.json 中需确保 `"externalConsole": flase` 即: 使用集成的控制台，而不是另外弹出弹窗的形式

然后 settings.json 中添加 msys2 的环境变量

```json
"terminal.integrated.env.windows": {
    "PATH": "C:\\ProgramFiles\\msys64\\ucrt64\\bin;${env:PATH}"
},
```

这样只要启动 cmd, msys2 的环境就会被引入, 从而调试的环境就有了， gdb 才能正常启动程序



#### 关于在 ssh 中无法运行 GUI 应用

提示 `could not connect to display`

```sh
# 也可尝试 := 1
export DISPLAY=:0
# 若还是不行, 则进入 UI 界面的 shell, 执行以下命令
# 作用是 access control disabled, clients can connect from any host
xhost +
```

#### Alpine Linux VScode 远程连接

参考: https://github.com/microsoft/vscode-remote-release/issues/6347

```sh
apk add bash curl git libstdc++ procps-ng
```

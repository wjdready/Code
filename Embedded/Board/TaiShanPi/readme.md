

# [2025-07-28 15:54:56]

立创教程: https://lceda001.feishu.cn/wiki/IJtRwVu5kiylHykl3RJcQ8ANncY

## flutter-pi 

```sh
# 使用 flutter_tool 构建
# 复制 ssh key, 从而免密码输入

# 添加 ssh 设备
flutter_tool device add shino@192.168.1.208

flutter_tool run -d shino@192.168.1.208
```

flutter_tool 会将应用放在目标主机 /tmp 中, 但是从 windows 中上传似乎没有自动添加可执行权限

因此添加如下脚本，自动添加执行权限

```sh
#!/bin/bash
# 监控/tmp目录下的文件创建和属性更改
inotifywait -m -r -e create -e attrib /tmp |
while read path action file; do
  if [[ "$file" == "flutter-pi" ]]; then
    if [[ ! -x "$path/$file" ]]; then
      chmod +x "$path/$file"
      echo "已为 $path/$file 添加执行权限"
    fi
  fi
done
```

## systemd

```sh
# 首先查看当前默认的 target：
systemctl get-default

# 切换到纯命令行模式（多用户模式）：
sudo systemctl isolate multi-user.target

# 切换回图形模式
sudo systemctl isolate graphical.target

# 如果要永久禁用图形界面（下次启动也不进入）：
sudo systemctl set-default multi-user.target

# 当您想恢复图形界面时：
# 或其他显示管理器如gdm、sddm
sudo systemctl start lightdm

# 或者永久恢复
sudo systemctl set-default graphical.target
```


## GPU 

https://cloud.tencent.com/developer/article/1571948

libmali: https://www.cnblogs.com/arnoldlu/p/18175082

```sh

# 通过观察是否有中断产生来判断 GPU 有没有在工作
cat /proc/interrupts | grep gpu

# 从节点名来搜索设备信息
find /sys -name *fde60000.gpu*
# 从而看到设备信息
cat /sys/devices/platform/fde60000.gpu/gpuinfo

# ls /dev/mali*
/dev/mali0
```



# 2023-05-06 13:45:48


参考 [Qemu\ARM64\01UbuntuBase] 搭建一个基本的 ubuntu 系统.

安装 psmisc (包含 pstree 命令) 来查看进程间的继承关系

Linux 启动时创建 0 号 IDLE 进程，并从用户空间启动 1 号进程, system 第一个被启动:

```
pstree -p -a -h -U

systemd,1
  ├─dbus-daemon,137 --system --address=systemd: --nofork --nopidfile --systemd-activation --syslog-only
  ├─dhcpcd,156
  ├─login,145 -f     
  │   └─bash,239
  │       └─bash,399
  │           └─demo,400
  ├─login,504 -f
  │   └─bash,518
  ├─networkd-dispat,133 /usr/bin/networkd-dispatcher --run-startup-triggers
  │   └─{networkd-dispat},159
  ├─sshd,139 -D
  │   ├─sshd,480
  │   │   └─bash,496
  │   │       └─pstree,617 -p -a -h -U
  │   └─sshd,580
  │       └─bash,596
  ├─systemd,209 --user
  │   └─(sd-pam),212
  ├─systemd-journal,114
  ├─systemd-logind,134
  ├─systemd-resolve,126
  └─systemd-timesyn,127
      └─{systemd-timesyn},129
```

从中可以看到, system 启动后依次启动了 dbus-daemon, dhcpcd, login 等一些列进程.

这些被启动后的进程再根据设置和用户操作来启动其后的子进程。比如上例中, 我得到这棵进程树是通过 pstree 来完成, 追其根源可以看到其父进程是 `bash,496`, 祖父进程是 `sshd,480`。

因为是通过 sshd 远程连接的, 它们都继承自 ssh 服务进程 `sshd,139`。可以看到我开了两个窗口登录了 ssh, 因此 `sshd,139` 下有两个子进程。

sshd 服务会检测 10021 端口, 当有用户登录时它会创建一个新的 sshd 进程来登录。

login 进程在开机后由 systemd 执行，用来启动用户登录界面，这里有两个是因为按照 `Qemu\ARM64\01UbuntuBase` 中做法:

```sh
ln -s /lib/systemd/system/getty@.service /etc/systemd/system/getty.target.wants/getty@ttyAMA0.service
vim /lib/systemd/system/getty@.service
```

getty@.service 是一个 getty 服务模板, 通过为其创建一个软连接, 放到 `/etc/systemd/system/` 下面, 来实现从串口登录;


## system service 示例 

/root/demo.sh

```
#!/bin/bash
while $(sleep 3); do
    echo "hello world"
done
```

/etc/systemd/system/hello.service

```s
[Unit]
Description=Hello World service
After=ssh.service

[Service]
Type=simple
ExecStart=/root/demo.sh

[Install]
WantedBy = multi-user.target
```

启动和查看状态

```sh
systemctl start hello
systemctl status hello

# 设置开机自动启动服务
systemctl enable hello
```

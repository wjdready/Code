
#### 安装 SSH 和 WSL
修改 C:/ProgramData/ssh/sshd_config
```shell
# 允许公钥授权访问，确保条目不被注释
# PubkeyAuthentication yes
# 授权文件存放位置，确保条目不被注释
# AuthorizedKeysFile	.ssh/authorized_keys
# 可选，关闭密码登录，提高安全性
# PasswordAuthentication no
# 注释掉默认授权文件位置，确保以下条目被注释
# Match Group administrators
#       AuthorizedKeysFile __PROGRAMDATA__/ssh/administrators_authorized_keys

type C:/ProgramData/ssh/sshd_config
echo PubkeyAuthentication yes >> C:\ProgramData\ssh\sshd_config
echo ssh公钥 > C:/ProgramData/ssh/administrators_authorized_keys
# 进入 powershell 重启sshd，需要管理员权限
powershell
Restart-Service sshd

# 如何设置环境变量, 例如
set BAT_HOME=c:\bat
setx PATH "%PATH%;D:\modules"

# 启用适用于 Linux 的 Windows 子系统：打开powershell并输入：
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart

wsl --status 
wsl --set-default-version 2
wsl --list --online
wsl --install -d Ubuntu-20.04

wget https://wslstorestorage.blob.core.windows.net/wslblob/wsl_update_x64.msi
msiexec /package 
```

### 远程连接 WSL
```shell
apt-get install xrdp openssh-server
service xrdp start
# 可查看端口和地址
cat /var/log/xrdp.log

# 所有用户有效
cd /usr/share/xsessions

# 当前用户
vim ~/.xsession
# 可在 xsession 中写入要启动的程序
# fluxbox, openbox, dwm, i3wm, fvwm, xterm, xclock
# 对于 xfce4
echo xfce4-session > ~/.xsession
# 默认打开浏览器
xinit chromium-browser $* -- :0 vt$XDG_VTNR 

# 测试 libdrm 
apt-get install libdrm-tests
export DISPLAY=localhost:10.0
```

#### 通过脚本控制, 进入 x11 后执行的脚本
vim ~/.xinitrc
```shell
#!/bin/sh

HOST=`uname -n | sed 's/\..*$//'`

xv -root -rmode 5 $HOME/misc/millennium/theme/Wallpaper.gif -quit &

xterm -geometry 80x24+0+85 \#52x71-104+0 -n $HOST -T $HOST &
xterm -geometry 80x24+510+429 \#52x71-52+0  -n $HOST -T $HOST &
xclock -digital -update 1 -geometry +1059+982 &
xscreensaver -nosplash &
exec /usr/local/lib/X11/fvwm/fvwm

DISPLAY=:21
vncserver -localhost -geometry 1920x1080  -SecurityTypes None $DISPLAY &
ssvnc $DISPLAY
xterm &
```

#### WIN 上的操作
```shell
# 需要windowss 放行端口 https://blog.csdn.net/weixin_54227557/article/details/126836097
netsh advfirewall firewall add rule name=WSL2 dir=in action=allow protocol=TCP localport=2202
netsh advfirewall firewall add rule name=WSL2 dir=in action=allow protocol=TCP localport=3389
netsh advfirewall firewall add rule name=WSL2 dir=in action=allow protocol=TCP localport=8888

# 端口映射
netsh interface portproxy add v4tov4 listenport=[win10端口] listenaddress=0.0.0.0 connectport=[虚拟机的端口] connectaddress=[虚拟机的ip]
netsh interface portproxy add v4tov4 listenport=2202 listenaddress=0.0.0.0 connectport=22 connectaddress=localhost
netsh interface portproxy add v4tov4 listenport=3389 listenaddress=0.0.0.0 connectport=3389 connectaddress=172.30.24.38
netsh interface portproxy add v4tov4 listenport=8888 listenaddress=0.0.0.0 connectport=8888 connectaddress=172.30.24.38

# 查看映射的端口
netsh interface portproxy show all

# 删除端口映射
netsh interface portproxy delete v4tov4 listenaddress=[监听地址] listenport=[监听端口]
netsh interface portproxy delete v4tov4 listenaddress=0.0.0.0 listenport=3389
netsh interface portproxy delete v4tov4 listenaddress=0.0.0.0 listenport=2202
netsh interface portproxy delete v4tov4 listenaddress=0.0.0.0 listenport=8888
```


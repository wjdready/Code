

# 注册一个用户

```sh
# -m 注册时创建用户目录
useradd shino -m
# 设置密码
passwd shino

# 使用 sudo 
apt-get install sudo
# 将用户添加到 sudo 组
usermod -a -G sudo shino
# 或者修改 /etc/sudoers 为用户设置权限
root    ALL=(ALL:ALL) ALL
shino   ALL=(ALL:ALL) ALL

# 更改某个文件的权限给用户
chown shino:shion 文件
```

# 自动登录

利用 systemd 自动登录某个终端, [参考](https://ostechnix.com/ubuntu-automatic-login/)

```sh
# 创建一个要自动登录的 tty, tty1 一般是屏幕下的终端
vim /etc/systemd/system/getty@tty1.service

[Service]
ExecStart=
ExecStart=-/sbin/agetty --noissue --autologin shino %I $TERM
Type=idle
```

# xserver

[Debian wiki](https://wiki.debian.org/Xorg#:~:text=Xorg%20is%20the%20default%20X,Current%20Status)
[Archlinux wiki](https://wiki.archlinux.org/title/Xinit#Autostart_X_at_login)
[Archlinux Systemd User](https://wiki.archlinux.org/title/Systemd/User#Automatic_login_into_Xorg_without_display_manager)

```sh
apt-get install xorg

# 执行 startx 后会启动 .xinitrc 或者.xsessionrc (.xsession) 
# 若存在 .xinitrc 则不启动后者, 不存在则 .xsessionrc 先启动再启动 .xsession
# 用户可自己设置要启动应用, 比如桌面系统 i3, 甚至是自己的程序 sdl2demo.
# 不论是 root 还是普通用户, 都可以启动. 不过非 root 用户需在屏幕终端中启动, 无法通过 ssh 启动 ？
vim ~/.xsession
i3

# 然后启动 xserver
startx
```

## 自动登录后启动 Xserver

在 ~/.bashrc 中添加如下代码, 则当登录进去后自动启动图像界面, 配合 systemd 的自动登录，可以实现不使用登录管理器的情况下自动进入图像界面

```sh
if [[ ! $DISPLAY && $XDG_VTNR -eq 1 ]]; then
    exec startx
fi
```

> 通过systemd 实现[自动登录](https://unix.stackexchange.com/questions/437809/start-x-server-on-login-with-systemd)

# 登录管理器

登录管理器一般都自带自动登录功能，还是比较方便的

```sh
# 1. gdm3 会自动下载整个 ubuntu 桌面?
apt-get install gdm3

# 2. lightdm
apt-get install lightdm

# 3. sddm
apt-get install sddm
# sddm 配置
vim /etc/sddm.conf
# 自动登录
# [Autologin]
# User=shion
# Session=i3

# 4. 公共
# 查看当前使用的显示管理器 
cat /etc/X11/default-display-manager

# 安装包可以重新配置
dpkg-reconfigure sddm

# 停止或启动登录管理器
systemctl stop sddm.service
```

# 桌面管理器

## i3 

[i3wm使用说明](https://www.jianshu.com/p/b9b644cf528f)

```sh
apt-get install i3

# i3 配置文件在 ~/.config/i3/config
vim ~/.config/i3/config

# 设置壁纸 feh 需要 apt 安装
exec_always --no-startup-id feh --bg-scale ~/bizhi.png
```

快捷键

* Mod + Enter：在当前的容器中打开一个终端
* Mod + d :在dmenu中启动应用
* Mod + w :全屏模式
* Mod + e :
* Mod + Shift + q：关闭当前容器中的应用程序
* Mod + j 或者 k：切换焦点（也就是将焦点移动到下一区域中的窗口或上一个窗口）
* Mod + 1...9：在工作环境的不同区域中移动焦点
* Mod + Shift + k 或者 j：将当前容器中的窗口向上或向下移动
* Mod + Shift + h 或者 l：将当前容器中的窗口分成左右两部分或者上下两部分
* Mod + Shift + Control + h 或者 l：将当前容器中的窗口分成两列或者两行


# 其他应用收集

文件管理 thunar nautilus
文本编辑器 gedit


# 其他心得

/etc/profile 每次登录都会执行一下


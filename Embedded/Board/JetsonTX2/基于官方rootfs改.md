
```sh
# 显示安装了哪些软件和其大小
dpkg-query -W --showformat='${Installed-Size}\t${Package}\n' | awk '{size=$1/1024; printf "%.2fMB\t%s\n", size, $2}' | sort -n

sudo tar -xf ../../packages/Tegra_Linux_Sample-Root-Filesystem_R32.7.3_aarch64.tbz2 -C rootfs

# 卸载这些软件
apt-get remove libreoffice-core docker

dpkg -l

# 卸载 gnome 相关的东西
apt-get purge gnome*

# 卸载不必要的软件
sudo apt-get autoremove

# 桌面管理器 lightdm gdm3

# 查看当前使用的显示管理器
cat /etc/X11/default-display-manager

# 切换其他界面系统
dpkg-reconfigure gdm3

ls /usr/share/xsessions/
apt-get purge openbox lxde-common

```





## weston

官方文档: https://wayland.pages.freedesktop.org/weston/toc/running-weston.html#

weston.ini 配置: https://zhuanlan.zhihu.com/p/396168706

```sh
# weston 合成器
apt-get install weston
sudo -E env "PATH=$PATH" XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR weston

# 编辑要启动的应用 vim  ~/.config/weston.ini, /etc/xdg/weston.ini
[autolaunch]
path=/path/to/your/application

# 创建 XDG_RUNTIME_DIR 目录?
sudo mkdir -p /run/user/$(id -u shino)
sudo chown shino:shino /run/user/$(id -u shino)
export XDG_RUNTIME_DIR=/run/user/$(id -u shino)
```


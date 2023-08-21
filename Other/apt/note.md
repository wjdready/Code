


如何在 x86 中使用 ARM 的 APT 仓库并通过交叉编译链接 ARM 仓库中的库

现在的做法是通过 Ubuntu-Base 构建一个 rootfs, 利用 QEMU 来 chroot 进去, 安装想要的内容, 交叉编译时可以使用 x86 的编译器, 链接时指定位置即可.

更高级的做法是完全使用主机的APT进行管理.

是不是可以使用 Windows 创建开发环境, pacman 来管理仓库, devkitpro 就是这样做的，但是需要维护自己的仓库。[archLinux](https://archlinuxarm.org/packages) 似乎是可以直接使用的吧...


# download 

下载某个包同时下载其所有依赖到本地

```sh
apt-get download $(apt-rdepends <package>|grep -v "^ ")
# 安装所有包
dpkg -i *.deb
```

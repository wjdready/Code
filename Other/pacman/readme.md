


pacman 仓库配置一般如下, 而 Include 的文件其实就是一系列的 Server = xxx, 就是一个简单的宏替换.

```conf
# [repo]
# Server = xxx

[mingw64]
Include = /etc/pacman.d/mirrorlist.mingw

[dkp-windows]
Server = https://pkg.devkitpro.org/packages/windows/$arch/
```


##### 包查找
```shell
# 只查看某个仓库的包
pacman -Sl mingw64

# 查找某个包, 支持正则, 比如 .*switch.*
pacman -Ss <regex string>
```



##### 从本地安装

```shell
pacman -U xxxx.tar.gz
```

##### 制作 pacman 软件包

参考 [ARCH 官方文档](https://wiki.archlinux.org/title/creating_packages)

通过官方文件的介绍我们知道， pacman 软件包使用 `makepkg` 命令来打包, 它使用一个名为 PKGBUILD 的包构建描述文件来从源代码创建二进制包.

makepkg 执行时会识别当前目录的 PKGBUILD 然后根据里面的描述进行打包, 下面是一个简单的 PKGBUILD 示例和模板:

PKGBUILD

```shell
# Maintainer: Your Name <youremail@domain.com>
pkgname=mydemopkg
pkgver=v1.0.0
pkgrel=1
pkgdesc=""
arch=()
url=""
license=('GPL')
groups=()
depends=()
makedepends=()
optdepends=()
provides=()
conflicts=()
replaces=()
backup=()
options=()
install=
changelog=
source=()
noextract=()
md5sums=() #autofill using updpkgsums

build() {
    echo "$pkgname-$pkgver"

}

package() {
    echo "$pkgname-$pkgver"
}
```

##### 执行打包和安装

现在 PKGBUILD 什么都没有, 但也是可以打包成功的.

```shell
ls 
# PKGBUILD

makepkg
# 这里输出了 mydemopkg-v1.0.0-1-x86_64.pkg.tar.zst

# 可选参数: 构建后清理临时文件
makepkg --clean

# 可选参数: 构建后安装
makepkg --install

# 使用 pacman 安装
pacman -U mydemopkg-v1.0.0-1-x86_64.pkg.tar.zst

# 卸载
pacman -U mydemopkg
```

生产的包实际上是一个 tar 压缩文件, 使用 zstd 压缩, 其包含如下内容

* 要安装的二进制文件。
* .PKGINFO：包含pacman处理包、依赖等所需的所有元数据。
* .BUILDINFO：包含可复制生成所需的信息。仅当使用 pacman 5.1 或更高版本构建程序包时，才会显示此文件。
* .MTREE：包含文件的散列和时间戳，这些散列和时间戳包含在本地数据库中，以便 pacman 可以验证包的完整性。
* .INSTALL：用于在安装/升级/删除阶段后执行命令的可选文件。(仅当在PKGBUILD中指定时，才会显示此文件)
* .Changelog：由包维护者保存的可选文件，用于记录包的更改。(并不是所有的包装中都有)

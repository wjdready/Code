
##### 创建 debian 包

参考[官方文档](https://wiki.debian.org/HowToPackageFordebian)

[参考文档](https://www.debian.org/doc/manuals/maint-guide/dreq.en.html)

##### 1. 创建 debian 包

例如下面是一个简单 hellopkg 包源文件结构:

```
hellopkg
├── DEBIAN
│   ├── control
└── opt
    └── hellopkg
        └── bin
            └── hellopkg.sh
```

debian 源包必须包含 DEBIAN 目录, 一般来说 DEBIAN 目录下会包含各种相关信息和构建规则, 其中 control 是必须包含的, 它给出了包的基本信息和依赖等.

例如 hellopkg 包的 control 文件如下:

```
Package: hellopkg
Version: 1.0.0
Maintainer: shino
Architecture: all
Description: example description
```

包中所包含的程序文件或配置文件都以最终安装位置的形式出现在包的根目录, 即假设包的根目录就是 Linux 的根, 则这里出现的除了 DEBIAN 以外的目录都将最终被原封不动地复制到系统根目录.

这里的程序就是一个简单的脚本, `hellopkg.sh` 安装后将被放到 `/opt/hellopkg/bin` 目录中.

注意程序文件是脚本的话需给执行权限:

```
chmod 755 hellopkg/opt/hellopkg/bin/hellopkg.sh
```

##### 2. 执行打包

```shell
# 生成 hellopkg.deb
dpkg-deb --build hellopkg hellopkg.deb

# 安装离线包
dpkg -i hellopkg.deb

# 执行脚本
/opt/hellopkg/bin/hellopkg.sh Hello World
# this is an example deb package
# here are some args: /opt/hellopkg/bin/hellopkg.sh, Hello, World, 
# that's all

# 卸载已安装的包
apt-get remove hellopkg
```


#### 更完整的包

[参考](https://wiki.opf-labs.org/display/SP/The+Hello+World+Debian+Package)


下面将以一个简单但又比较全面的包构建过程为例

需要安装 debhelper 工具
```
sudo apt-get install debhelper

```

##### 1. changelog

主要包含有关版本号、修订、分发和紧迫性的信息。[（更多信息）](http://www.debian.org/doc/manuals/maint-guide/dreq.en.html#changelog)

```
hello-world (1.0.0) unstable; urgency=low

  * Initial Release.

 -- Helder Silva <hsilva@keep.pt>  Wed, 08 Feb 2012 09:59:32 +0000
```

##### 2. compat 

指定兼容模式。如今，它从 1（默认值）变为 9，每个都指定某些任务的特定行为，这些任务在构建过程的特定目标中执行。[（更多信息）](http://www.debian.org/doc/manuals/maint-guide/dother.en.html#compat)。

```
8
```

##### 3. control 

包含包的重要信息，即包名、维护者、依赖项（构建依赖项、执行依赖项）等，这些信息对包相关程序很有用：dpkg、apt-get 等。

```
Source: hello-world
Section: misc
Priority: extra
Maintainer: Hélder Silva <hsilva@keep.pt>
Build-Depends: debhelper (>= 8.0.0), perl
Standards-Version: 3.9.2
Homepage: http://www.keep.pt/

Package: hello-world
Architecture: all
Depends: ${misc:Depends}
Description: This is a dummy package with a hello world echo script
 This is the long description...
 This is the long description...
 This is the long description...
 This is the long description...
 This is the long description...
```

##### 4. copyrights 

包含有关与“程序”文件和包相关文件相关的版权和许可的信息[（更多信息）](http://www.debian.org/doc/manuals/maint-guide/dreq.en.html#copyright)。

```
Format: http://dep.debian.net/deps/dep5
Upstream-Name: hello-world
 
Files: *
Copyright: 2012 Hélder Silva <hsilva@keep.pt>
License: LGPL-2.0+
 
Files: debian/*
Copyright: 2012 Hélder Silva <hsilva@keep.pt>
License: LGPL-2.0+
 
License: LGPL-2.0+
 This package is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later version.
 .
 This package is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 .
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 .
 On Debian systems, the complete text of the GNU Lesser General
 Public License can be found in "/usr/share/common-licenses/LGPL-2".
```

##### 5. dirs 

包含有关必须存在或在包安装时创建的目录的信息[（更多信息）](http://www.debian.org/doc/manuals/maint-guide/dother.en.html#dirs)。

```
usr/bin
```

##### 6. hello-world.manpages

指定将要安装的手册页文件的名称

```
debian/hello-world.1
```

##### 7. hello-word.pod

使用 POD（Perl 旧文档）格式描述的手册页，将在 Debian 软件包创建过程中转换为 nroff 格式

```
=head1 NAME
 
hello-world - This is a dummy package with a hello world echo script.
 
=head1 SYNOPSIS
 
hello-world
 
=head1 DESCRIPTION
 This is the long description...
 This is the long description...
 This is the long description...
 This is the long description...
 This is the long description...
 
=head1 AUTHORS
 
B<hello-world> was written by Helder Silva <hsilva@keep.pt>.
 
This manual page was written by Helder Silva <hsilva@keep.pt>.
=cut
```

##### 8. install 

描述 hello-world 脚本安装目录的文件[（更多信息）](http://www.debian.org/doc/manuals/maint-guide/dother.en.html#install)。

```
hello-world usr/bin/
```

##### 9. README

包含有关程序的信息。

```
The Debian Package hello-world
----------------------------
 
Comments regarding the Package
 
 -- Helder Silva <hsilva@keep.pt>  Wed, 08 Feb 2012 09:59:32 +0000
```

##### 10. rules 

用于包生成的 Debian 特定 makefile[（更多信息）](http://www.debian.org/doc/manuals/maint-guide/dreq.en.html#rules)。

```
#!/usr/bin/make -f
 
%:
    dh $@
 
override_dh_clean:
    dh_clean
    rm -f debian/hello-world.1
override_dh_auto_build:
    dh_auto_build
    pod2man --section=1 --release=$(VERSION) --center "" debian/hello-world.pod > debian/hello-world.1
```

##### 11. format 

说明软件包格式的 Debian 文件。对于 Debian 本地软件包，它必须是“3.0 (native)”，对于其他所有软件包，它必须是“3.0 (quilt)”（更多信息）。

```
3.0 (native)
```

##### 12. hello-word.sh

我们的脚本程序

```
#!/bin/sh
 
echo "Hello world!"
```

#### 执行打包

```shell
cd hello/hello-world_1.0.0/
dpkg-buildpackage

# 使用 lintian 验证包
cd ../
lintian hello-world_1.0.0_all.deb

# 安装
sudo dpkg -i hello-world_1.0.0_all.deb

# 运行
hello-world
```
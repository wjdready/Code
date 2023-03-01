
### [2023-02-25 19:13:38]

[官方文档](https://devkitpro.org/wiki/Getting_Started), 对于 MSYS2 环境可以[看这篇](https://devkitpro.org/wiki/devkitPro_pacman).

devkitPro 提供的工具和库由相当出色的Arch Linux pacman管理. 首先设置环境导出, 可将下列函数添加到 bashrc, 然后按需导出.

```shell
# DEVKITPRO 环境导出
function export_DEVKITPRO
{
    export DEVKITPRO=/opt/devkitpro
    export DEVKITARM=/opt/devkitpro/devkitARM
    export DEVKITPPC=/opt/devkitpro/devkitPPC
}
```

下载和安装密钥包

```shell
wget https://pkg.devkitpro.org/devkitpro-keyring.pkg.tar.xz
pacman -U devkitpro-keyring.pkg.tar.xz
```

添加 devkitPro 存储库, 编辑 /etc/pacman.conf 并添加这些行， 并更新 `pacman -Syu`

```shell
[dkp-libs]
Server = https://pkg.devkitpro.org/packages

[dkp-windows]
Server = https://pkg.devkitpro.org/packages/windows/$arch/
```

devkitpro 提供了一些方便开发的包集合 (Groups):

```
- gp32-dev
- gp2x-dev
- gba-dev 
- nds-dev
- 3ds-dev
- gamecube-dev
- wii-dev
- wiiu-dev
- switch-dev
```

因为是 switch 开发, 这里选 `switch-dev` 即可.

```
switch-dev
pacman -S devkitARM
```

### [2023-02-25 20:37:40]

有了这个开发环境我们可以做些什么呢? 可以看看 examples, 编译一个图形界面玩玩

```shell
# 别忘记导出环境 export_DEVKITPRO
cd /opt/devkitpro/examples/switch/graphics/sdl2/sdl2-demo

# 安装 sdl2
pacman -S switch-sdl2 switch-sdl2_gfx switch-sdl2_image switch-sdl2_mixer switch-sdl2_net switch-sdl2_ttf
make
```

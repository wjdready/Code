

```sh
# 解压 ramdisk.img
mkdir ramdisk && cd ramdisk
cat ../ramdisk.img | cpio -idmv

# 对于 uboot 引导的, uInitrd, 可先去掉 uboot 头
dd if=/mnt/sdcard/uInitrd of=initrd bs=64 skip=1
# 查看文件, 可知 lz4 压缩
file initrd
mkdir initrd && cd initrd_out
# 解压
lz4 -dc ../initrd_out | cpio -idmv
```


## RK35XX Pro

使用 ubuntu22 


#### 分析 systemd

```sh
# 切换到纯命令行模式（多用户模式）：
sudo systemctl isolate multi-user.target
# 切换回图形模式
sudo systemctl isolate graphical.target

# 1. 系统启动流程分析
# 查看启动耗时
systemd-analyze
# 查看启动耗时排行
systemd-analyze blame
# 查看启动耗时链
systemd-analyze critical-chain

# 查看启动流程图（生成SVG）
systemd-analyze plot > boot.svg

# 2. 系统单元和依赖关系
# 列出所有单元
systemctl list-units --all
# 查看特定单元的依赖关系
systemctl list-dependencies <unit>
# 查看单元文件路径
systemctl show <unit> -p FragmentPath

# 4.进程树和调用关系
# 查看完整的进程树（包含 cgroup 信息）
systemd-cgls
# 更详细的进程关系（需要安装）
pstree -p 1  # 从PID 1开始显示进程树
# 实时监控系统状态
systemd-cgtop

# 5.深入分析工具
# 查看系统状态快照
systemctl status
# 检查单元配置
systemd-analyze verify <unit-file>
# 调试模式启动（临时）
systemd.log_level=debug
```


## 启动流程

发现进入桌面是 launcher.service

而 launcher.service 是由 systemd-sysv-generator 自动生成的

```sh
# 查看转换后的服务文件
systemctl cat launcher.service
# 查看生成器配置
cat /usr/lib/systemd/system-generators/systemd-sysv-generator

# .dge 后缀应用, 其实就是 elf 可执行文件重命名

# /mnt/vendor/bin/dmenu.bin 是主界面程序

# 图形显示相关设备，没有 /dev/dri/card0 难道是专用驱动? 不是通用的 DRM 框架
/dev/mali0
/dev/disp


```

### arm64 运行 armhf

```sh
sudo dpkg --add-architecture armhf

# 必须执行 update 才更新 armhf 包
sudo apt-get update

# 安装 armhf 的包使用 :armhf 后缀
# 安装的库放在 /usr/lib/arm-linux-gnueabihf/

# libc6 安装后 放在 /usr/lib/arm-linux-gnueabihf
# 安装后会自动创建链接文件 /lib/ld-linux-armhf.so.3 从而提供 armhf 的运行基础
sudo apt install libc6:armhf 
```


### sdl2 示例

```sh
g++  sdl2_gles_demo1.cpp -lSDL2 -lGLESv2
ldd a.out 
# 发现依赖 /usr/lib/libSDL2-2.0.so.0
# 删除后依赖 /lib/aarch64-linux-gnu/libSDL2-2.0.so.0 仍能运行
# 也就是说, SDL2 是通用的, 在不使用 drm, x11, wayland 的情况下, SDL2 和 OpenGL 仍能使用
# SDL2 是如何让画面起来, 提供 OpenGL 上下文的?
# 发现运行时，有一个叫 mali-fbdev 的窗口管理器? 如果直接在graphical.target中运行，则会报错
root@ANBERNIC:~# ./drmtest 
# 窗口创建失败: mali-fbdev: Can't create EGL window surface

# 在泰山派上运行 (也是非 graphical.target)，则是 SDL_Init 时就失败了
root@MiWiFi-RA81-srv:/root# ./drmtest 
# error: XDG_RUNTIME_DIR not set in the environment.
# SDL初始化失败: No available video device

同样都是 ubuntu 中下载的 libsdl2 库

# 使用 qt
```


## EGL 测试


```sh
# 运行 glmark2-es2 报错 Error: eglGetDisplay() failed with error: 0x3000
# glmark2 则发现驱动是 GL_RENDERER:   llvmpipe
# 显然没有加载使用显卡

# 查找所有 GL 库
find / -name lib*GL*.so*
# 是有 /usr/lib/libEGL.so 库的, 但可能版本过低, 导致 eglGetDisplay 失败

# 编译测试程序， 保证链接的是 /usr/lib/libEGL.so, 可能存在 /usr/lib/aarch64-linux-gnu/libEGL.so 
# 这时 llvmpipe 软件渲染库，可先删除
gcc -o egl_test egl_test.c -lEGL
# 此时显示 egl_test.c:(.text+0x20): undefined reference to `eglGetDisplay' 说明确实没有该函数

# 如果删掉 /usr/lib/libEGL.so 恢复 /usr/lib/aarch64-linux-gnu/libEGL.so
# 则能编译通过, 但是 glmark2-es2 显示的是软件渲染

# 查看 gpu 信息
find /sys -name *gpu*
cat /sys/devices/platform/gpu/gpuinfo
# Mali-G31 1 cores r0p0 0x7093

# 从网上找到 Mali-G31 代码并编译 https://github.com/Hancock33/batocera.piboy/blob/aa4d527f9c7429493a808643e9a67ece74b439ea/package/batocera/gpu/mali/mali-G31/mali-G31.mk

# 返回错误 #define EGL_BAD_PARAMETER 0x300C

```

```c
// egl_test.c
#include <EGL/egl.h>
#include <stdio.h>

// gcc -o egl_test egl_test.c -lEGL

int main() {
    EGLDisplay display;
    EGLint major, minor;

    // 1. 获取默认显示
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        printf("eglGetDisplay failed: 0x%x\n", eglGetError());
        return -1;
    }

    // 2. 初始化EGL
    if (!eglInitialize(display, &major, &minor)) {
        printf("eglInitialize failed: 0x%x\n", eglGetError());
        return -1;
    }
    printf("EGL initialized: version %d.%d\n", major, minor);

    // 3. 获取配置
    EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs)) {
        printf("eglChooseConfig failed: 0x%x\n", eglGetError());
        return -1;
    }

    printf("EGL test completed successfully!\n");
    eglTerminate(display);
    return 0;
}
```


## flutter 

```sh
# 使用设备自带的 libEGL 库, 运行不通过, 使用 mesa 的 llvmpipe 软件渲染库可以运行
# 将 /usr/lib 里面关于 GL 部分删除, 只需删除软链接即可, 比如 /usr/lib/libEGL.so 和 /usr/lib/libEGL.so.1
# 查看 flutter 程序所使用的库 ldd demo | grep lib.*GL.*, 如果还有依赖基于 /usr/lib 目录的 GL 库则继续删除
# 直到可以观察 flutter 使用的是安装的 mesa 库的 GL 库 (在 /usr/lib/aarch64-linux-gnu)
# 怀疑是 EGL 驱动不全或者不最新，缺少某些函数实现导致，后面可尝试下载最新的 EGL 驱动编译并替换
```


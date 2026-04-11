

## 安装 kernelsu:

https://kernelsu.com/

主要就是提取系统固件中的 init_boot.img, 然后由 kernelsu 打上 root补丁, 再通过 flashboot 重新刷入即可

#### 具体流程如下:

1. 首先要查看系统版本信息, 然后下载对应的固件，然后从固件的 payload.bin 中提取 init_boot.img

不过以下地址已经提供了 init_boot.img, 直接下载对应版本的就行了

https://www.kdocs.cn/l/cdJKdxE8JOE3


2. 然后将 init_boot.img 拷贝至手机, 打开 kernelsu app 选择安装, 找到 init_boot.img, 点击安装后生成打了补丁的 kernelsu_patched_xxx.img

3. 将 kernelsu_patched_xxx.img 拷贝到电脑, 使用以下命令安装

```sh
adb reboot bootloader
fastboot flash init_boot kernelsu_patched_xxx.img
```

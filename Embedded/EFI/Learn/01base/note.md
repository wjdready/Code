
UEFI 可执行文件格式为 PE32+ 格式。和 Linux 的可执行文件 ELF 格式不同，而 gcc 默认生成 ELF 格式

PE32+为 64 位 Windows 的可执行文件格式，32 位的为 PE32

gcc 参数 --subsystem,10 可生成可执行文件类型为 PE32+格式，所欲我们得到 UEFI 可执行文件

* -Wall ：开启所有警告 
* -Wextra ：开启额外的警告 
* -e efi_main ：指定入口函数为  efi_main  
* -nostdinc ：不包含标准头文件目录 
* -nostdlib ：不使用标准库 
* -fno-builtin ：禁止使用内建函数 
* -Wl,--subsystem,10 ：传递链接器参数，设置子系统为  10  

UEFI 加载器会初始化屏幕磁盘等简单设备，然后载入 UEFI 程序，传入结构体指针。从而在我们的结构 efi_main 中可以操作这些指针进行相应操作

## 加载 efi 程序

UEFI 可以识别 FAT 格式的文件系统。因此，我们首先需要把 U 盘格式化成 FAT32 格式

将编译好的 EFI 程序放到启动磁盘的 EFI/BOOT/BOOTX64.EFI 下， 开机后加载器就会自动加载了

如果设备支持 EFI Shell，也可以在 shell 中调用 EFI，以下是一个 EFI Shell 操作示例：
```sh
# 列出所有的被 UEFI 识别的文件系统
map -r

# 进入某个文件系统
fs0:

```

## 在 qemu 上运行

QEMU 默认不带有 UEFI 固件，我们需要手动安装 OVMF(Open Virtual Machine Firmware) 软件包。

QEMU 有将目录挂载为虚拟 FAT 驱动器的功能。例如，创建一个名为 esp 的目录，将 efi 文件放到 esp/EFI/BOOT/BOOTX64.EFI 并使用-drive file=fat:rw:esp,index=0,format=vvfat 选项运行 QEMU。

虚拟机将会把 esp 目录认作是一个 FAT 驱动器分区，并会自动启动到其中的 BOOTX64.EFI 文件。

OVMF自身不带鼠标驱动，需要手动加载鼠标驱动。鼠标驱动也是 efi 程序




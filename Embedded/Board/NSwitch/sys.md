

# [2023-10-28 12:00:23]

破解芯片会从 SD卡 (exFat 格式) 加载 payload.bin 

目前广泛使用的第三方是 hekate https://github.com/CTCaer/hekate

CTCaer 的这个 payload.bin 作为第一个从 sd 卡加载的 bootloader, 其功能很多，而且它还可以加载其他的 payload.bin, 下面是其用到的 bootloader 目录的详细介 (摘自 github) 

## Bootloader folders and files

| Folder/File              | Description                                                           |
| ------------------------ | --------------------------------------------------------------------- |
| bootloader               | Main folder.                                                          |
|  \|__ bootlogo.bmp       | It is used if no `logopath` key is found. User provided. Can be skipped. |
|  \|__ hekate_ipl.ini     | Main bootloader configuration and boot entries in `Launch` menu.      |
|  \|__ nyx.ini            | Nyx GUI configuration                                                 |
|  \|__ patches.ini        | Add external patches. Can be skipped. A template can be found [here](./res/patches_template.ini) |
|  \|__ update.bin         | If newer, it is loaded at boot. Normally for modchips. Auto updated and created at first boot. |
| bootloader/ini/          | For individual inis. `More configs` menu. Autoboot is supported.   |
| bootloader/res/          | Nyx user resources. Icons and more.                                   |
|  \|__ background.bmp     | Nyx - Custom background. User provided.                               |
|  \|__ icon_switch.bmp    | Nyx - Default icon for CFWs.                                          |
|  \|__ icon_payload.bmp   | Nyx - Default icon for Payloads.                                      |
| bootloader/sys/          | hekate and Nyx system modules folder.                                 |
|  \|__ emummc.kipm        | emuMMC KIP1 module. !Important!                                       |
|  \|__ libsys_lp0.bso     | LP0 (sleep mode) module. Important!                                   |
|  \|__ libsys_minerva.bso | Minerva Training Cell. Used for DRAM Frequency training. !Important!  |
|  \|__ nyx.bin            | Nyx - hekate's GUI. !Important!                                       |
|  \|__ res.pak            | Nyx resources package. !Important!                                    |
|  \|__ thk.bin            | Atmosphère Tsec Hovi Keygen. !Important!                              |
| bootloader/screenshots/  | Folder where Nyx screenshots are saved                                |
| bootloader/payloads/     | For the `Payloads` menu. All CFW bootloaders, tools, Linux payloads are supported. Autoboot only supported by including them into an ini. |
| bootloader/libtools/     | Reserved                                                              |


## switchroot

```sh
# barrier 安装
# synergy 有个 bug 就是鼠标光标不会动
# barrier 2.4 似乎也有这个问题，有人反应过 issues https://github.com/debauchee/barrier/issues/589 
# 但是后来从 https://github.com/debauchee/barrier/issues/42 中找到原因，竟然是屏幕缩放问题，在 Windows 中设置 100% 缩放就好了
# 但 synergy 似乎仍然存在问题，所以以后可能用 barrier 比较多，这里是 ubuntu 下 barrier 源码安装的一些必须步骤
git clone https://github.com/debauchee/barrier.git
git submodule init
sudo apt install libcurl4-openssl-dev libavahi-compat-libdnssd-dev libxtst-dev qtbase5-dev libssl-dev
mkdir build -p && cd build && cmake ..
make -j8 
sudo make install

# 安装完后可以直接命令行启动
barrierc -f --no-tray --disable-crypto  --debug INFO --name  ns_switch 192.168.55.100:24800

sudo usermod -md /home/NewHome -l NewName OldName 
sudo groupmod -n NewGroupName OldGroupName 
```

大气层系统

git clone https://github.com/Atmosphere-NX/Atmosphere.git _Atmosphere

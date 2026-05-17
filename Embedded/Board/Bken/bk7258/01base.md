
官方文档: https://docs.bekencorp.com/
文档下载: https://dl.bekencorp.com/
官方GIT: https://github.com/bekencorp

有好几个工程, 比如 bk_idk, bk_ai_dk, 以及各种 bk_solution_xx, 还有一个 armino

比较乱, 不知道应该使用哪个, armino 看起来很久没更新过了. bk_solution_xx 可能是专门针对某个解决方案的出的完整demo, 有官方配套开发板

目前先看看 bk_idk

bk_idk文档:
https://docs.bekencorp.com/arminodoc/bk_idk/bk7258/zh_CN/v2.0.2/introduction/index.html

bk_idk 使用 Ubuntu 开发

```sh
# ref https://docs.bekencorp.com/arminodoc/bk_idk/bk7258/zh_CN/v2.0.2/get-started/env-manual.html

# 切换到最新版本
git reset --hard v2.0.2.2

# 安装环境, 会自动下载并安装 gcc 
sudo bash tools/env_tools/setup/armino_env_setup.sh

# 编译默认 app 工程
make bk7258 PROJECT=app

# 对于 windows 下载 Armino-Bash-Setup, 里面已经集成各种工具，安装后只需进入 Armino-Bash 命令行即可编译运行

# 烧录 从 https://dl.bekencorp.com/tools/bkfil 下载, 然后写入 all_app.bin
# 可以使用 cli 版本
# 基本烧录命令（需要指定串口）
./tools/bk_loader.exe download -p COM3 -i build/usbtest/bk7258/all-app.bin
# 完整烧录命令（带重启和擦除）
./tools/bk_loader.exe download -p COM3 -b 2000000 -r -e 1 -i build/usbtest/bk7258/all-app.bin
```


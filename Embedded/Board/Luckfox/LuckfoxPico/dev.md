
# [2023-11-20 12:23:17]

官方 [wiki](https://wiki.luckfox.com/zh/Luckfox-Pico/Luckfox-Pico-quick-start)

## 源码编译

```sh
# 
git clone https://gitee.com/LuckfoxTECH/luckfox-pico.git

./build.sh -h
```



构建脚本的框架类似 AOSP, 最主要的是 project 目录, 包含 build.sh, 工程目录下的 build.sh 只是 project/build.sh 的软链接

首先 `build.sh lunch` 选择板子, 然后 `build.sh [option]` 来选择要构建的目标

project/cfg-all-items-introduction.txt 给出了所有配置项

根据 build.sh 中

```sh
RK_TARGET_BOARD_ARRAY=( $(cd ${TARGET_PRODUCT_DIR}/; ls BoardConfig*.mk BoardConfig_*/BoardConfig*.mk | sort) )
```

可知每个板子的配置都在 cfg 下, 必须以 BoardConfig 为前缀



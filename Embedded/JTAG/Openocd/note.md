

Windows msys2 环境下编译

```sh
# 安装编译依赖
pacman -S autoconf automake libtool pkg-config make gcc

# 编译失败如下报错时
# ' is already registered with AC_CONFIG_FILES.
# 需要修改 configure.ac 中的 AC_CONFIG_FILES 中去掉 \ 换行, 改成单行空格分开

# 编译 openocd
./bootstrap
mkdir build && cd build
../configure \
  --prefix=$PWD/out \
  --enable-ftdi \
   --enable-internal-jimtcl
```


## 直接下载 Release 

官方版本基于 mingw 体积较大 https://github.com/openocd-org/openocd/releases

sysprogs 编译的版本: https://gnutoolchains.com/arm-eabi/openocd/


## Reset 规则

```sh
reset_config 
          [none|trst_only|srst_only|trst_and_srst]
          [srst_pulls_trst|trst_pulls_srst|combined|separate]
          [srst_gates_jtag|srst_nogate]
          [trst_push_pull|trst_open_drain]      
          [srst_push_pull|srst_open_drain]
# 
#                   1. 复位模式
# trst              Test Reset 专用于 JTAG 调试接口的复位(复位 JTAG 扫描链), SWD 模式下几乎不用
# srst              System Reset 系统复位, 用于复位整个系统, 包括所有外设和核心
# none	            禁用硬件复位(仅用软件复位, 如写寄存器 0xE000ED0C)	芯片无硬件复位引脚, 或复位引脚被占用
# trst_only	        仅用 TRST 复位(仅 JTAG 模式)
# srst_only	        仅用 SRST 复位(仅复位芯片系统)
# trst_and_srst	    同时用 TRST+SRST 复位
# 
#                   2. 引脚联动规则
# srst_pulls_trst   触发 SRST 时, 自动拉低 TRST(SRST 主导)
# trst_pulls_srst   触发 TRST 时, 自动拉低 SRST(TRST 主导)
# combined	        TRST/SRST 是同一个物理引脚(复用)
# separate	        TRST/SRST 是独立引脚(默认)
# 
#                   3. SRST 对 JTAG 的控制
# srst_gates_jtag	SRST 复位时, 关闭 JTAG 接口(JTAG 暂停工作)
# srst_nogate	    SRST 复位时, JTAG 接口仍工作(默认)
# 
#                   4. TRST 电气特性(仅 JTAG 有用)
# trst_push_pull	TRST 为推挽输出(能输出高低电平, 驱动能力强)
# trst_open_drain	TRST 为开漏输出(需上拉电阻, 仅能拉低电平)
# 
#                   5. SRST 电气特性(SWD/M23 核心配置)
# srst_push_pull	SRST 为推挽输出(调试器主动输出高 / 低电平)	  芯片 NRST 引脚无内部上拉, 或需要强驱动
# srst_open_drain	SRST 为开漏输出(仅能拉低, 高电平依赖外部上拉电阻)	芯片 NRST 有内部上拉, 避免电平冲突
# 

cortex_m reset_config ['sysresetreq'|'vectreset']
# sysresetreq    系统复位请求(System Reset Request): 复位整个芯片(内核 + 外设 + 时钟)
# vectreset      向量复位(Vector Reset): 仅复位内核核心(PC 跳回复位向量), 外设保持不变
```


arm-none-eabi-gcc
 -o build\kernel\components\drivers\sdio\sdio.o
 -c
 -march=armv7-a
 -marm
 -msoft-float
 -Wall
 -Werror
 -g
 -gdwarf-2
 -O0
 -DHAVE_CCONFIG_H
 -D__RTTHREAD__
 -DRT_USING_NEWLIB
 -DRT_USING_LIBC
 -D_POSIX_C_SOURCE=1
 -I.
 -Iapplications
 -Idrivers
 -Irt-thread\include
 -Irt-thread\libcpu\arm\common
 -Irt-thread\libcpu\arm\cortex-a
 -Irt-thread\components\dfs\include
 -Irt-thread\components\dfs\filesystems\devfs
 -Irt-thread\components\dfs\filesystems\elmfat
 -Irt-thread\components\dfs\filesystems\ramfs
 -Irt-thread\components\dfs\filesystems\romfs
 -Irt-thread\components\drivers\include
 -Irt-thread\components\drivers\spi
 -Irt-thread\components\drivers\spi\sfud\inc
 -Irt-thread\components\finsh
 -Irt-thread\components\libc\compilers\common\include
 -Irt-thread\components\libc\compilers\newlib
 -Irt-thread\components\libc\cplusplus
 -Irt-thread\components\libc\posix\delay
 -Irt-thread\components\libc\posix\io\aio
 -Irt-thread\components\libc\posix\io\poll
 -Irt-thread\components\libc\posix\io\stdio
 -Irt-thread\components\libc\posix\io\termios
 -Irt-thread\components\libc\posix\ipc
 -Irt-thread\components\libc\posix\pthreads
 -Irt-thread\components\libc\posix\signal
 -Irt-thread\components\lwp
 -Irt-thread\components\net\lwip\lwip-2.0.3\src\include
 -Irt-thread\components\net\lwip\lwip-2.0.3\src\include\ipv4
 -Irt-thread\components\net\lwip\lwip-2.0.3\src\include\netif
 -Irt-thread\components\net\lwip\port
 -Irt-thread\components\net\netdev\include
 -Irt-thread\components\net\sal\include
 -Irt-thread\components\net\sal\include\socket
 -Irt-thread\components\net\sal\impl
 -Irt-thread\components\net\sal\include\dfs_net
 -Irt-thread\components\net\sal\include\socket\sys_socket rt-thread\components\drivers\sdio\sdio.c


make qemu 开启仿真，其中 -s 参数用于启用仿真服务, 默认端口为 1234

这时 gdb 打开 elf 文件, 连接到服务器 `target extended-remote localhost:3333` 即可

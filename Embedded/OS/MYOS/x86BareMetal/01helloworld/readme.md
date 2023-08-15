
存在的问题:

nasm 似乎无法 debug.

MingW 的 ld 无法链接成 elf 格式, 默认只有 pe pep 格式，
在 windows 下使用 as 和 gcc 编译是正常输出的，这时对输出的目标文件，在 linux 下再使用 ld 进行链接可执行。
说明仅仅是只是链接器的问题，要在 windows 下链接，可能需要专门的链接工具. 

即使已经成功链接成 elf 文件, 使用 MingW 默认的 gdb 调试也无法成功, 提示 Remote target doesn't support qGetTIBAddr packet

目前是在 windows 启动 gdb server, 在 ssh 虚拟机中远程连接调试。

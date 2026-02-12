
T-Head DebugServer 下载
https://xuantie.t-head.cn/community/download?id=4202772147627692032

安装后在 bin\links\CK-Link 目录下有最新的 cklink_lite.hex

将 CKLinkLite.hex 转为 STM32F103 固件
https://github.com/cjacker/cklink-lite-fw-convertor/

CKLinkLite.hex 原厂固件烧录地址是 0x08004000 而 stm32f103 烧录地址是 0x08000000, 所以把 0x08004000 - 0x080040F0 部分地址拷贝出来, 放到 0x08000000 - 0x080000F0

其目的是让 stm32 复位后默认从 0x08000004 地址处取的 PC 值能直接转跳到正常执行位置, 其实只需改 0x08000000 - 0x08000004 就可以了, 因为转跳过后就正常执行了, 因为固件中不会用到 0x08004000 之前的地址

对应 stm32f103 引脚
A0	TRST
A1	TCK
A4	TDO
A5	TMS
B9	TDI
3V3	3V3
GND	GND

# 2026-01-21 12:54:33
发现校验和 < 10 时没有自动填充 0, 需修改 intelchksum.py 中  `str(hex(chksum)[2:])` 为 `f"{chksum:02x}"`

# 2026-01-21 13:23:17

ref: http://www.360doc.com/content/24/0420/21/19438193_1120966940.shtml

其实  0x08000000 - 0x08004000 这个地址是存放 bootloader 的, 因为支持自动更新, 所以 hex 起始位置是 0x08004000

对应的固件是 cklink_lite_iap.hex, 只需烧录这个固件, 打开 T-Head DebugServer 就可以更新到最新的了


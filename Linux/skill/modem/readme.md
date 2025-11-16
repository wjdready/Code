
### modem-manager 拨号

https://andino.systems/andino-4g-modem/setup-via-modem-manager

### PPP 拨号

参考: https://blog.csdn.net/weixin_45121946/article/details/107284629

拨号脚本:

```sh
# /etc/ppp/peers/rasppp

# Usage: sudo pppd call rasppp

# 连接调试时隐藏密码
hide-password

# 该手机不需要身份验证4
noauth

# 用于呼叫控制脚本
connect '/usr/sbin/chat -s -v -f /etc/ppp/peers/rasppp-chat-connect'

# 断开连接脚本
disconnect '/usr/sbin/chat -s -v -f /etc/ppp/peers/rasppp-chat-disconnect'

# 调试信息
debug

# 4G模块
/dev/ttyUSB2

# 串口波特率
115200

# 使用默认路由
defaultroute

# 不指定默认IP
noipdefault

# 不使用PPP压缩
novj
novjccomp
noccp
ipcp-accept-local
ipcp-accept-remote
local

# 最好锁定串行总线
lock
dump

# 保持pppd连接到终端
nodetach

# 用户名 密码
# user
# password
#移动，联通拨号不需要用户名密码，文末给出不同运营商的配置

# 硬件控制流
crtscts
remotename 3gppp
ipparam 3gppp

# 请求最多两个DNS服务器地址
usepeerdns
```


rasppp-chat-connect


```sh
# /etc/ppp/peers/rasppp-chat-connect
# 连续15秒，收到以下字符，则退出执行
TIMEOUT 15
ABORT   "BUSY"
ABORT   "ERROR"
ABORT   "NO ANSWER"
ABORT   "NO CARRTER"
ABORT   "NO DIALTONE"

# 40秒内没有收到指定字符，则退出
# 例如 OK \rATZ,发送ATZ，希望收到的是OK
""AT
OK \rATZ

# 建立连接，联通为3gnet,文末给出各运营商配置
OK \rAT+CGDCONT=1,"IP","cmnet"

# 拨号,*99#是联通的拨号号码
OK-AT-OK ATDT*98*1#
CONNECT \d\c
```


```sh
/etc/ppp/peers/rasppp-chat-disconnect
ABORT "ERROR"
ABORT "NO DIALTONE"
SAY "\NSending break to the modem\n"
""\k"
""+++ATH"
SAY "\nGood bye !\n"
```

开始拨号

```sh
sudo pppd call rasppp

# ifconfig 可以看到 ppp0

# 添加路由
route add default dev ppp0

# 联网测试
# ping 8.8.8.8

# ping 域名如果失败，请添加DNS
# 从 /etc/ppp/resolv.conf 可以查看到当前运营商的 DNS
# 将其添加到 /etc/resolv.conf
```


```sh
# /etc/ppp/peers/dial
#
# pppd options
#
modem
debug
#logfile /var/ppplog 
nodetach 
/dev/ttyUSB2
115200
receive-all
defaultroute
noipdefault
-chap
usepeerdns ##使用pppoe的DNS,否则改/etc/resolv.conf文件
#mtu 1500
-crtscts
ipcp-accept-local
ipcp-accept-remote
lock
noauth
novj
nobsdcomp
novjccomp
nopcomp
noaccomp
asyncmap 0
noccp
# user "card"  ####设置用户名
# password "card" ####设置密码

#lcp-echo-failure 0
#lcp-echo-interval 0

# Chat scripts
connect "/usr/sbin/chat -v -s -f /etc/ppp/chat"
```

```sh
# /etc/ppp/chat
# 
# chat script 
# 
# 
	ABORT		'BUSY'
	ABORT		'NO ANSWER'
	ABORT		'NO CARRIER'
	ABORT		'NO DIALTONE'
	ABORT		'\nRINGING\r\n\r\nRINGING\r'
	SAY			"modem init: press <ctrl>-C to disconnect\n"

	''		'+++ATH'
#	OK		'ATZ'
#	OK		'ATQ0 V1 E1 S0=0 &C1 &D2 +FCLASS=0'
#	OK		'AT+IFC=0,0'
#	OK		'AT&W'
	SAY		"Before Connecting\n"

#	OK		'AT+CSOCKAUTH=,,"card","card"'
	OK		'AT+CGDCONT=1,"IP","cmnet"'
	SAY		"\n + defining PDP context\n"

#	OK		'ATD#777'
	OK		"ATD*98*1#"
	SAY		"Number Dialled\n"

#	OK		'AT+CGATT=1'
	SAY		"\n + attaching"
# simcom
#	OK		'AT+CIPSTART'
#	SAY		"\n + attaching to GPRS"

	SAY		"\n + requesting data connection"
#	CONNECT		''
	CONNECT		\d\c
	SAY		"\n + connected"
```

### 移远quectel-CM拨号工具

https://github.com/Carton32/quectel-CM

CFLAGS += -Wno-unused-result -Wno-error=stringop-overflow


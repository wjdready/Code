

## 虚拟机添加串口

```sh
# ubuntu.vmx
# 使用命名管道实现双向通信
serial0.present = "TRUE"
serial0.fileType = "pipe"
serial0.fileName = "\\.\pipe\vmware_serial"
serial0.pipe.endPoint = "server"
serial0.yieldOnMsrRead = "TRUE"
serial0.tryNoRxLoss = "FALSE"

# /etc/default/grub
GRUB_CMDLINE_LINUX="console=tty0 console=ttyS0,115200n8"
GRUB_TERMINAL="console serial"
GRUB_SERIAL_COMMAND="serial --speed=115200 --unit=0 --word=8 --parity=no --stop=1"

# 执行
sudo update-grub
sudo systemctl enable serial-getty@ttyS0.service
sudo systemctl start serial-getty@ttyS0.service

# 在 host 端连接到命名管道
putty -serial \\.\pipe\vmware_serial -sercfg 115200,8,n,1,N
```

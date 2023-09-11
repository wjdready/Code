

partclone 在进行分区备份时只复制已经使用的部分, 方便系统备份

```sh
sudo apt-get install partclone

# 克隆分区为镜像 -c clone -s 源文件 -o 输出文件
partclone.ext4 -c -s /dev/sda1 -o sda1.img

# 将镜像恢复到分区 -r 恢复 
partclone.ext4 -r -s sda1.img -o /dev/sda1

# 或者直接可以分区到分区克隆 -b 逻辑设备到逻辑设备
partclone.ext4 -b -s /dev/sda1 -o /dev/sdb1

# 显示镜像信息
partclone.info -s sda1.img

# 检查镜像
partclone.chkimg -s sda1.img
```

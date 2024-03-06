


```sh
sudo apt-get install vsftpd
# 默认无需配置直接连就能用。访问的文件夹是 user 的 home 目录
sudo vim /etc/vsftpd.conf
# write_enable=YES      #允许用户上传文件
sudo systemctl restart vsftpd
```

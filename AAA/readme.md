

```sh
# pip 临时镜像源
pip install some-package -i https://pypi.tuna.tsinghua.edu.cn/simple

# npm 临时镜像源
npm install some-package --registry https://registry.npm.taobao.org

# npm 设置永久镜像源
npm set registry https://registry.npm.taobao.org
npm config set registry https://registry.npmjs.org
npm get registry 

# 下载 cnpm
npm install -g cnpm --registry=https://registry.npmmirror.com

# 查看某个包的历史版本
npm view <package-name> versions
npm info <package-name> 

# 系统时间同步网络时钟, 然后同步到硬件时钟
sudo ntpdate cn.pool.ntp.org
sudo hwclock --systohc 

```


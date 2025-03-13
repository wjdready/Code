

```sh
# ssh 免密登录 .ssh/authorized_keys

# git 配置
git config --global user.name "your_username"
git config --global user.email "your_email"

# pip 临时镜像源
pip install some-package -i https://pypi.tuna.tsinghua.edu.cn/simple

# 系统时间同步网络时钟, 然后同步到硬件时钟
sudo ntpdate cn.pool.ntp.org
sudo hwclock --systohc 

# 安卓 repo 
curl https://mirrors.tuna.tsinghua.edu.cn/git/git-repo -o repo
export REPO_URL='https://mirrors.tuna.tsinghua.edu.cn/git/git-repo'
```

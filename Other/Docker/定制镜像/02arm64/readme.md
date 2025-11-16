

```sh
apt-get install qemu-user-static

# 
docker run --rm --privileged multiarch/qemu-user-static:register

# 拉取ARM64 Docker镜像
docker pull ubuntu:20.04 --platform=arm64

# 运行 arm64 Docker容器
docker run -it -v /usr/bin/qemu-aarch64-static:/usr/bin/qemu-aarch64-static ubuntu:20.04 /bin/bash -c "uname -m; exec /bin/bash"
# sed -i 's/ports.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list

# arm32
docker pull ubuntu:20.04 --platform=arm
docker run -it -v /usr/bin/qemu-arm-static:/usr/bin/qemu-arm-static ubuntu:20.04 /bin/bash -c "uname -m; exec /bin/bash"

```


## 保存修改内容

使用 commit 提交修改后的镜像
```sh
# 查看刚刚修改的容器 ID
docker ps -a

# 提交修改, 并创建新镜像
docker commit -m "add net-tools" -a "wjdready <wjdready@163.com>" 0861c73ee471  ubuntu:20.04_1

# 查看提交历史
docker history  ubuntu:20.04_1
```


## VSCode

```sh

docker run 

docker run -d \
  --hostname gitlab.example.com \
  -p 443:443 -p 80:80 -p 22:22 \
  --name gitlab \
  --restart always \
  -v /your/local/path/gitlab:/etc/gitlab \
  gitlab/gitlab-ce:latest
```



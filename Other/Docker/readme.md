
Docker Desktop: https://github.com/tech-shrimp/docker_installer/releases/download/latest/docker_desktop_installer_windows_x86_64.exe

安装后将 Docker\Docker\resources\bin 添加到 PATH

```sh

# https://github.com/dongyubin/DockerHub

# 打开 Docker Desktop 设置->Docker Engine 添加如下配置, 然后 Apply
{
  "registry-mirrors": [
    "https://docker.1ms.run"
  ]
}
```


```sh
docker run -itd --name ubuntu-test ubuntu
```

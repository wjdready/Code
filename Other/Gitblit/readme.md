

[下载](https://www.gitblit.com/)

依赖 [JDK8~11](https://download.java.net/java/GA/jdk11/9/GPL/openjdk-11.0.2_windows-x64_bin.zip)

配置文件 安装目录\data\defaults.properties

修改 `server.httpPort` 为某一个端口, 默认为 0, 需要设置 http 端口才能访问, 默认 https 端口为 8443, 然后运行 `gitblit.cmd` 开启服务

用户配置在 `data\users.conf` 文件中, 默认管理员 `admin`, 密码 `admin`


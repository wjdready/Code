

## npm 使用

```sh
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
```


## nvm 安装

### 1. linux版本

[sh版本](https://github.com/nvm-sh/nvm)

### 2. windows版本

```sh
# 地址: https://github.com/coreybutler/nvm-windows/releases

# 1. 下载免安装版本, 解压后在解压目录下创建 settings.txt 如下
root: C:\ProgramFiles\Library\nvm
arch: 64
proxy: none
originalpath: .
originalversion: 
node_mirror: https://npmmirror.com/mirrors/node
npm_mirror: https://npmmirror.com/mirrors/npm

# 2. 设置环境变量并导出, %NVM_SYMLINK% 和 %NVM_SYMLINK% 到 PATH
# [注] NVM_SYMLINK 其实是一个目录快捷方式, 由 nvm 自动创建, 指向当前 nodejs 版本
NVM_HOME 为 C:\ProgramFiles\Library\nvm
NVM_SYMLINK 为 C:\ProgramFiles\Library\nvm\nodejs
```

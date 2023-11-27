


# 创建一个 npm 包

```sh
mkdir demo && cd demo
npm init -y
```


```sh
npm set registry https://registry.npm.taobao.org
npm config set registry https://registry.npmjs.org
npm get registry 

npm install -g cnpm --registry=https://registry.npmmirror.com
cnpm install --save-dev electron
cnpm sync express

npm install --save-dev electron

# 查看某个包的历史版本
npm view <package-name> versions
npm info <package-name> 
```


node 模块安装位置不在 nodejs 安装目录，而是默认在 `C:\Users\shino\AppData\Roaming` 


## npm 版本快速切换

官方地址:
https://github.com/coreybutler/nvm-windows
https://github.com/nvm-sh/n

使用参考:
https://zhuanlan.zhihu.com/p/653505351


settings.txt:
```sh
root: C:\ProgramFiles\Library\nvm-noinstall
arch: 64
proxy: none
originalpath: .
originalversion: 
node_mirror: https://npmmirror.com/mirrors/node
npm_mirror: https://npmmirror.com/mirrors/npm
```

```sh
# 设置环境变量
NVM_HOME = C:\ProgramFiles\Library\nvm-noinstall
NVM_SYMLINK = C:\ProgramFiles\Library\nodejs

# 阿里云镜像
nvm npm_mirror https://npmmirror.com/mirrors/npm
nvm node_mirror https://npmmirror.com/mirrors/node
# 腾讯云镜像
nvm npm_mirror http://mirrors.cloud.tencent.com/npm/
nvm node_mirror http://mirrors.cloud.tencent.com/nodejs-release/
```

## npm 快速切换源

```sh
# 安装 cgr 来快速切换源
npm install -g cgr
cgr ls
cgr use taobao
```

## nodejs 版本 与 NODE_MODULE_VERSION 的关系

[参考官网](https://nodejs.org/en/download/releases/#backref-1) 


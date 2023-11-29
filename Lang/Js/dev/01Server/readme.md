
如何快速创建一个 HttpServer 用于测试

## 使用 nodejs 内置 http 模块

server.js

```js
const http = require('http');
const fs = require('fs');
const path = require('path');

const server = http.createServer((req, res) => {
    let filePath = decodeURIComponent(req.url);
    if (filePath.endsWith('/')) {
        filePath += 'index.html';
    }
    filePath = path.join(__dirname, 'public', filePath);
    console.log(filePath);
    fs.readFile(filePath, (err, data) => {
        if (err) {
            if (err.code === 'ENOENT') {
                res.statusCode = 404;
                res.end('File not found');
            } else {
                res.statusCode = 500;
                res.end('Internal server error');
            }
        } else {
            if (req.url.match(/.js$/)) {
                res.setHeader('Content-Type', 'text/javascript');
            }
            res.statusCode = 200;
            res.end(data);
        }
    });
});

const port = 3000;
server.listen(port, () => {
    console.log(`Server running at http://localhost:${port}/`);
});
```
> 然后直接运行 node server.js

## 使用 express

```
npm init
npm install express
node server.js
```

server.js

```js
const express = require('express');
const app = express();
const path = require('path');

// 指定静态文件目录
const staticPath = path.join(__dirname, 'public');
app.use(express.static(staticPath));

const port = 3000;
app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}/`);
});
```

## npx serve

直接在当前文件夹下 `npx serve`

## Live Server

VScode 安装 Live Server 插件，在 html 中直接 右击->OpenWithLiveServer


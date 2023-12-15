
# 创建工程

```sh
npm create tauri-app
npm install
npm run tauri dev
```

采用 Vue + Tauri的话，最外层是 Vue, 嵌入 src-tauri

src-tauri 是 tauri 源码位置，基于 Rust Cargo，其中 `tauri.conf.json ` 是配置 tauri 的关键

所有配置项参考官方文档: https://tauri.app/v1/api/config/

例如初始化后配置文件如下:

```json
{
  "build": {    // 提供构建流程的自定义化，可配置构建前执行命令，构建时输出路径等
    "beforeDevCommand": "pnpm dev",
    "beforeBuildCommand": "pnpm build",
    "devPath": "http://localhost:1420",
    "distDir": "../dist"
  },
  "package": {  // 提供打包信息设置
    "productName": "tauri-app",
    "version": "0.0.0"
  },
  "tauri": {    // tauri 的各项配置
    "allowlist": {
      "all": true,
      "shell": {
        "all": false,
        "open": true,
      }
    },
    "bundle": {
      "active": true,
      "targets": "all",
      "identifier": "com.tauri.dev",
      "icon": [
        "icons/32x32.png",
        "icons/128x128.png",
        "icons/128x128@2x.png",
        "icons/icon.icns",
        "icons/icon.ico"
      ]
    },
    "security": {
      "csp": null
    },
    "windows": [
      {
        "fullscreen": false,
        "resizable": true,
        "title": "Demo",
        "width": 800,
        "height": 600
      }
    ]
  }
}
```

最外层配置是

```
package:        Package settings
tauri:          The Tauri config
build:          The build configuration
plugins:        The plugins config
```

其中 tauri 的配置是重点, 它也提供了几种配置项:

```
pattern:        The pattern to use.
windows:        The windows configuration.
cli:            The CLI configuration.
bundle:         The bundler configuration.
allowlist:      The allowlist configuration.
security:       Security configuration.
updater:        The updater configuration.
systemTray:     Configuration for app system tray.
macOSPrivateApi:
```

具体参考官网，一个技巧是查看官网右边的目录可以清晰地看到各个配置分布和快速导航到各个配置信息说明位置。

特别是配置 tauri->allowlist 中描述了 tauri 官方所提供的各种 js 接口，下面我们将对这些接口进行尝试使用

# 使用接口

目前提供的接口组件有:

```
fs
window
shell
dialog
http
notification
globalshortcut
os
path
protocol
process
clipboard
app
```

然后接口组件 API 使用参考地址: https://tauri.app/v1/api/js/

在接口 API 中同样值得注意的是右边栏中的目录结构, 它可快速定位一个组件 API 的各种用法位置.

对于快速测试组件来说可以直接在接口组件同级下添加 `"all": true` 即可默认使用所有接口

## fs

该模块防止路径遍历，不允许绝对路径或父目录模式 （即不允许 `/usr/path/to/file` 或 `../path/to/file` 路径）, 使用此 API 访问的路径必须相对于**基本目录**之一 

因此如果需要访问任意文件系统路径，则必须在核心层上自行编写此类逻辑

基本目录参考[官方文档](https://tauri.app/v1/api/js/fs/#basedirectory)

此外还必须在配置文件中设置文件访问范围，没有列出的范围应用将没有访问权限:

```json
{
  "tauri": {
    "allowlist": {
      "fs": {
        "scope": ["$APPDATA/**"]
      }
    }
  }
}
```

例如读取一个目录 `readDir`:

```js
import { readDir, BaseDirectory } from '@tauri-apps/api/fs';
// Reads the `$APPDATA/users` directory recursively
const entries = await readDir('users', { dir: BaseDirectory.AppData, recursive: true });

function processEntries(entries) {
  for (const entry of entries) {
    console.log(`Entry: ${entry.path}`);
    if (entry.children) {
      processEntries(entry.children)
    }
  }
}
```
## 



一个能让 C/C++ 转成 Js 运行在网页的开发库 https://emscripten.org/index.html

根据官方介绍进行[安装](https://emscripten.org/docs/getting_started/downloads.html)

```
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
```

之后将安装目录下的 `upstream\emscripten` 导出到环境即可

## 入门

```sh
# 将输出 a.out.wasm 和一个用于加载 wasm 的 a.out.js
emcc hello.c
node a.out.js

# 生成用于测试的 html 
emcc hello.c -o hello.html

# -c 生成中间文件
emcc -c hello.c

# 链接到 SDL2 静态库, 若没有则会自动下载源码现场编译成 wasm 静态库, 也是 .a 结尾
emcc hello.o -s USE_SDL=2 -o hello.js
```

## js 中调用 wasm

```c
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
int add(int n)
{
    return n+1;
}
```

```sh
emcc -O3 -s WASM=1 -s EXPORTED_RUNTIME_METHODS='["cwrap"]' add.c
```

```html
<script src="a.out.js"></script>
<script>
    Module.onRuntimeInitialized = _ => {
        const add = Module.cwrap('add', 'number', ['number']);
        console.log(add(12));
    };
</script>
```

参考文献
https://web.dev/articles/emscripting-a-c-library

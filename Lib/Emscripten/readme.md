
一个能让 C/C++ 转成 Js 运行在网页的开发库 https://emscripten.org/index.html

根据官方介绍进行[安装](https://emscripten.org/docs/getting_started/downloads.html)

```
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
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

add.c

```c
#include <emscripten.h>

// EMSCRIPTEN_KEEPALIVE 宏保证函数不会被编译器优化掉
EMSCRIPTEN_KEEPALIVE
int add(int n)
{
    return n+1;
}
```

```sh
emcc -O3 -s WASM=1 -s EXPORTED_RUNTIME_METHODS=cwrap add.c -o add.js
```

web 中调用, 需运行本地服务, 比如 `python -m http.server`

```html
<script src="add.js"></script>
<script>
    Module.onRuntimeInitialized = _ => {
        const add = Module.cwrap('add', 'number', ['number']);
        console.log(add(12));
    };
</script>
```

nodejs 中调用

```js
const Module = require('./add.js');

Module.onRuntimeInitialized = () => {
    const add = Module.cwrap('add', 'number', ['number']);
    console.log(add(12));
};
```

## C 中内联 js

```c
#include <emscripten.h>

EM_JS(
void, draw_rectangle, (int x, int y, int width, int height, const char* color), {
    const ctx = Module.canvas.getContext('2d');
    ctx.fillStyle = UTF8ToString(color);
    ctx.fillRect(x, y, width, height);
});

int main() {
    // 初始化canvas
    EM_ASM({
        Module.canvas = document.getElementById('canvas');
    });
    
    // 绘制红色矩形
    draw_rectangle(50, 50, 100, 100, "red");
    
    return 0;
}

// 编译
// emcc hello.c -o hello.html -sEXPORTED_FUNCTIONS=_main -sEXPORTED_RUNTIME_METHODS=ccall,cwrap
```

参考文献
https://web.dev/articles/emscripting-a-c-library

---
mytags: myblog
title: 
date: 2023-01-17 22:07:42
categories: []
tags: []
---

本文主要介绍
<!-- more -->

一个简单的 SDL2 程序
main.c
```c
// #define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    SDL_Window *window = SDL_CreateWindow("demo", SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool quit = false;
    SDL_Event event;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEMOTION:
                if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1))
                    SDL_RenderDrawPoint(renderer, event.motion.x, event.motion.y);
                break;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 60);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    return 0;
}
```

编译
```sh 
gcc main.c -lmingw32 -lSDL2main -lSDL2
```

若去掉 `-lmingw32 -lSDL2main` 则需要在 `#include <SDL2/SDL.h>` 之前添加 `#define SDL_MAIN_HANDLED` 以避免找不到 `WinMain`

* 使用 pkg-config, 如果通过包管理器安装 sdl2, 在下载的同时一般都会附加 pkg-config 的 .pc 配置文件, 比如 MSYS2 下

```sh
# 下载 ucrt64 的 SDL2 包
pacman -S mingw-w64-ucrt-x86_64-SDL2
# 则有
find /ucrt64/lib -name "*sdl2*pc*"
# 输出: /ucrt64/lib/pkgconfig/sdl2.pc
```
这时, 使用 pkg-config 查看包配置的输出

```sh
pkg-config sdl2 --cflags --libs
# 输出: -IC:/ProgramFiles/msys64/ucrt64/include/SDL2 -Dmain=SDL_main -lmingw32 -mwindows -lSDL2main -lSDL2 
```

`-lmingw32` 和 `-lSDL2main` 上面直接编译的一样, pkg-config 默认附加的 `-mwindows` 用于启动时禁止终端弹出

于是

```sh
gcc main.c `pkg-config sdl2 --cflags --libs`
```

* 使用 cmake

同样的通过包管理器安装 sdl2 还附加了 cmake 包的配置, 也是在 lib 下,
```sh 
find /ucrt64/lib -name "*SDL2*"
# 输出:
# /ucrt64/lib/cmake/SDL2
# /ucrt64/lib/cmake/SDL2/SDL2Config.cmake
# /ucrt64/lib/cmake/SDL2/SDL2ConfigVersion.cmake
# /ucrt64/lib/cmake/SDL2/SDL2mainTargets-release.cmake
# /ucrt64/lib/cmake/SDL2/SDL2mainTargets.cmake
# /ucrt64/lib/cmake/SDL2/SDL2staticTargets-release.cmake
# /ucrt64/lib/cmake/SDL2/SDL2staticTargets.cmake
# /ucrt64/lib/cmake/SDL2/SDL2Targets-release.cmake
# /ucrt64/lib/cmake/SDL2/SDL2Targets.cmake
# /ucrt64/lib/cmake/SDL2/SDL2testTargets-release.cmake
# /ucrt64/lib/cmake/SDL2/SDL2testTargets.cmake
# /ucrt64/lib/libSDL2.a
# /ucrt64/lib/libSDL2.dll.a
# /ucrt64/lib/libSDL2main.a
# /ucrt64/lib/libSDL2_test.a
```

> 可以看到 Cmake 的话有多种配置可供选择

直接使用默认配置的 CmakeLists.txt 如下:

```cmake
cmake_minimum_required(VERSION 3.10)
project(temp)

# 可执行文件
add_executable(a main.c)

# 链接库和依赖
find_package(SDL2 REQUIRED SDL2)
target_link_libraries(a ${SDL2_LIBRARIES})
```

> 通过 find_package 查找包, 然后通过 ${<包名>_LIBRARIES} 进行引用

* 静态链接

通过 `find /ucrt64/lib -name "*SDL2*"` 可以看到 .a 文件, 说明下载的 SDL2 包是包含静态链接库的, 我们试图直接使用 -static 来编译, 发现出现很多 `undefined reference to __imp` 说明除了 libSDL2.a 外还需要其他静态库.

通过 pkg-config sdl2 --cflags --libs --static 可以得到所有依赖库, 于是静态编译指令为

```sh
gcc main.c `pkg-config sdl2 --cflags --libs --static` -static
```

<!-- gcc main.c  -Wl,-Bstatic -lSDL2  -Wl,-Bdynamic -->

Cmake 静态链接在SDL配置规则里也已经集成, 只需将库换成 `SDL2::SDL2-static`, 即 `target_link_libraries(a SDL2::SDL2-static)`并且需要在 `#include <SDL2/SDL.h>` 之前添加 `#define SDL_MAIN_HANDLED`, 猜测可能是静态库默认去掉了 `-lmingw32 -lSDL2main` 选项, 而添加 SDL_MAIN_HANDLED 后如果没有定义 WinMain 则 SDL 库会帮我们定义, 总之加上是可以兼容面对两种情况的.


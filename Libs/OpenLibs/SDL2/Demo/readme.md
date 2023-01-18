---
mytags: myblog
title: SDL2 Hello World
date: 2023-01-17 22:07:42
categories: [SDL2, 入门]
tags: [SDL2]
---

本文主要介绍 SDL2 的基本使用.
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

SDL_MAIN_HANDLED 在 sdl_main.h 中有如下定义
```c
#ifndef SDL_MAIN_HANDLED
#if defined(__WIN32__)
/* On Windows SDL provides WinMain(), which parses the command line and passes
   the arguments to your main function.
   If you provide your own WinMain(), you may define SDL_MAIN_HANDLED
 */
#define SDL_MAIN_AVAILABLE
 
//...........
 
#endif
```
并且
```C
#if defined(SDL_MAIN_NEEDED) || defined(SDL_MAIN_AVAILABLE)
#define main    SDL_main
#endif
 
/**
 *  The prototype for the application's main() function
 */
extern C_LINKAGE int SDL_main(int argc, char *argv[]);
```

也就是说默认情况下没有定义 `SDL_MAIN_HANDLED` 的话, `#include <SDL2/SDL.h>` 就会把 main 宏替换成 SDL_main, 而宏在编译链接之前替换, 所以在链接时就会发现找不到 main 函数(或说 WinMain), 而如果链接到 `-lmingw32 -lSDL2main` 则不存在该问题(需要放在 -lSDL2 前面)，因为 -lSDL2main 提供了 main 函数. 这时候实际的 man 函数其实不是在自己定义源文件里, 而是在 libSDL2main 里.

而如果我们定义了 SDL_MAIN_HANDLED 就是要求使用我们自己的 main 函数就，不要发生宏替换，这种情况可以直接使用 `-lSDL2` 

```sh
gcc main.c -lSDL2
```

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

`-lmingw32 -lSDL2main` 和上面直接编译的一样, pkg-config 默认还附加的 `-mwindows` 用于启动时禁止终端弹出

于是

```sh
gcc main.c `pkg-config sdl2 --cflags --libs`
```

* 使用 cmake

同样的通过包管理器安装的 sdl2 还附加了 cmake 包的配置, 也是在 lib 下,

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

使用 `message(${SDL2_LIBRARIES})` 查看 cmake 变量, 可以看到 `${SDL2_LIBRARIES}` 值为 `SDL2::SDL2mainSDL2::SDL2`, 默认链接到了 SDL2main

* 静态链接

通过 `find /ucrt64/lib -name "*SDL2*"` 可以看到 .a 文件, 说明下载的 SDL2 包是包含静态链接库的, 我们试图直接使用 -static 来编译, 发现出现很多 `undefined reference to __imp` 说明除了 libSDL2.a 外还需要其他依赖库才能进行静态编译, 而具体需要哪些, 我们目前无法得知.

于是尝试通过 `pkg-config sdl2 --cflags --libs --static` 发现可以得到所有依赖库, 即

```sh
pkg-config sdl2 --cflags --libs --static
# 输出: -IC:/ProgramFiles/msys64/ucrt64/include/SDL2 -Dmain=SDL_main -lmingw32 
# -mwindows -lSDL2main -lSDL2 -lmingw32 -mwindows -lSDL2main -lSDL2 -lm -luser32 
# -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lversion -luuid -ladvapi32 -lsetupapi -lshell32 -ldinput8
```

于是静态编译指令为

```sh
gcc main.c `pkg-config sdl2 --cflags --libs --static` -static
```

Cmake 静态链接在 SDL 配置规则里也已经集成, 只需将 SDL2 库换成 `SDL2::SDL2-static`:

```cmake
cmake_minimum_required(VERSION 3.10)
project(temp)

# 可执行文件
add_executable(a main.c)

# 链接库和依赖
find_package(SDL2 REQUIRED SDL2)
target_link_libraries(a SDL2::SDL2main SDL2::SDL2-static)
```

如果不使用 `SDL2::SDL2main` 即只链接到 `SDL2::SDL2-static`: `target_link_libraries(a SDL2::SDL2-static)` 则需要在 `#include <SDL2/SDL.h>` 之前定义 `#define SDL_MAIN_HANDLED` 以禁止宏替换.

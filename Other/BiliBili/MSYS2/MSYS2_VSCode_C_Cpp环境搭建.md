---
mytags: myblog
title: MSYS2 + VSCode 在 windows 上搭建 C/C++ 开发环境
date: 2023-09-25 08:24:33
categories: [Bilibili, MSYS2]
tags: [MSYS2, 开发环境]
---

本文主要介绍如何使用 MSYS + VSCode 在 windows 上搭建 C/C++ 开发环境
<!-- more -->

# 1. 安装和配置 MSYS2

* 官网下载并安装

* 启动脚本 msys.bat

  ```bat
  @C:\msys64\msys2_shell.cmd -ucrt64 -defterm -no-start -full-path  -here
  ```

* 换镜像源

  ```shell
  sed -i "s#https\?://mirror.msys2.org/#https://mirrors.tuna.tsinghua.edu.cn/msys2/#g" /etc/pacman.d/mirrorlist*
  ```

* 安装编译器和调试器

  ```sh
  pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb
  ```

# 2. 安装 VSCode 和配置

* 官网下载

* 安装 C/C++ 插件

* 以下是在 user settings.json 中配置

* 设置 C_Cpp 编译器路径 

  ```json
  "C_Cpp.default.compilerPath": "C:\\msys64\\ucrt64\\bin\\gcc.exe"
  ```

* 设置默认终端为 MSYS2

  ```json
  "terminal.integrated.defaultProfile.windows": "MSYS2",
  "terminal.integrated.profiles.windows": {               
  "MSYS2": { 
          "path": [ "C:\\Library\\Command\\msys.bat" ],
          "args": [ "" ],
          "icon": "terminal-cmd"
      },
  },
  ```

# 3. 在 VSCode 中调试程序

* 创建 launch.json

* 主要设置 program 和 miDebuggerPath

  ```json
  {
      "version": "0.2.0",
      "configurations": [
          {
              "name": "(gdb) 启动",
              "type": "cppdbg",
              "request": "launch",
              "program": "${workspaceFolder}/helloworld/a.exe",
              "args": [],
              "stopAtEntry": false,
              "cwd": "${fileDirname}",
              "environment": [],
              "externalConsole": true,
              "MIMode": "gdb",
              "miDebuggerPath": "C:\\msys64\\ucrt64\\bin\\gdb.exe",
              "setupCommands": [
              ]
          }
      ]
  }
  ```

  

# 4. 基于第三方库开发

## 示例

安装 SDL2 

```
pacman -S mingw-w64-ucrt-x86_64-SDL2 pkg-config
```

demo.c

```c
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    SDL_Window *window = SDL_CreateWindow("demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool quit = false;
    SDL_Event event;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEMOTION:
                if(SDL_GetMouseState(NULL, NULL) &SDL_BUTTON(1))
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

```
gcc demo.c `pkg-config SDL2 --cflags --libs --static` -static
```


---
title: 基于 rootfs 的交叉编译环境
date: 2023-09-03 09:53:03
categories: [交叉编译, Ubuntu]
tags: [交叉编译]
---

本文主要介绍如何构建基于 Ubuntu-Base rootfs 的交叉编译环境
<!-- more -->


利用另一篇文章中的 `Linux\Distro\Ubuntu\ubuntu-base-rootfs.sh` 可以快速构建一个 ubuntu rootfs.

然后通过 chroot 安装我们想要的库，例如 SDL2 这时便搭建好编译环境了。

然后进行编译测试

main.c

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


Makefile 

```makefile
CC = aarch64-linux-gnu-gcc
CXX = aarch64-linux-gnu-g++

ROOTFS = /home/shino/code/Linux/source_debug/_rootfs

CFLAG += $(LIBS) $(INCLUDES) 
CFLAG += --sysroot=$(ROOTFS)

all:
	$(CC) main.c $(CFLAG)  -lSDL2 -o sdl2demo
```


执行 make 后可以看到编译生成 aarch64 的可执行文件, 关键点是编译器的 --sysroot 参数，该参数告诉了编译器链接时应该去哪个地方搜索库。

# [2023-09-03 09:48:29]

此方法还是存在一些局限性，不知为何有些库链接失败，例如 opencv。会提示很多 .so 里的函数 undefine, 尚不知如何解决

另一种做法是直接使用 apt 仓库安装其他架构的软件包，可查看该主题下的另一篇文章.

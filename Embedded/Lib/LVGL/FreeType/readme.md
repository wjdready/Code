


在 LVGL 中使用 FreeType


[FreeType 下载](https://download.savannah.gnu.org/releases/freetype/)

```sh
wget https://download.savannah.gnu.org/releases/freetype/freetype-2.10.0.tar.gz

tar -xf freetype-2.10.0.tar.gz

# 或者仿真的话直接按照 pacman 仓库里面的
pacman -S ucrt64 mingw-w64-ucrt-x86_64-freetype

# 添加 lv_lib_freetype
git clone https://github.com/lvgl/lv_lib_freetype
```


cmake 中加载 pkg-config 库[参考](https://cmake.org/cmake/help/v3.5/module/FindPkgConfig.html)


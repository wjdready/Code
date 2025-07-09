


vcpkg.exe install qt5-base --triplet x86-windows

静态链接：
https://devblogs.microsoft.com/cppblog/vcpkg-updates-static-linking-is-now-available/
https://stackoverflow.com/questions/70807164/how-to-set-vcpkg-static-for-every-platform

qt5
https://blog.csdn.net/u011843342/article/details/132961547
upx
https://www.cnblogs.com/jiftle/p/17932723.html


https://learn.microsoft.com/zh-cn/vcpkg/users/triplets


心得:
安装时下载慢，可以将链接复制到 github 代理网站，然后下载后放到 vcpkg downloads 目录, 再重新执行安装命令

## 基本使用

```sh
# 搜索
vcpkg search sdl2
# 安装静态版本 (没有 :x64-windows-static 默认安装动态版本) 或者 x86 安装64位版本
vcpkg install sdl2:x64-windows-static
# 安装完成后会提示使用方法，知道一个库的用法也可使用 install package 进行查看
# sdl2 provides CMake targets:

#     find_package(SDL2 CONFIG REQUIRED)
#     target_link_libraries(main
#         PRIVATE
#         $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
#         $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
#     )

# 对于生成 msvc 32位
cmake -B build -G "Visual Studio 17 2022" -A Win32
cmake --build build --config Release
# 或者在 cmake 中设置
# if(WIN32)
#     set(CMAKE_GENERATOR_PLATFORM Win32 CACHE STRING "Platform" FORCE)
# endif()
```

```python
cmake_minimum_required(VERSION 3.10)

# 设置 vcpkg 工具链文件路径和目标三元组即可使用 vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/ProgramFiles/Library/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows-static")

project(HelloWorld)

set(CMAKE_BUILD_TYPE "Release")

find_package(SDL2 REQUIRED)

if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found. Please ensure it is installed via vcpkg.")
else()
    message(STATUS "SDL2 found: ${SDL2_LIBRARIES}")
endif()

add_executable(HelloWorld main.cpp)

target_link_libraries(HelloWorld PRIVATE SDL2::SDL2main SDL2::SDL2-static)

# TRUE 为 WIN32 GUI 程序, 不启动控制台, 默认 FALSE
set_target_properties(HelloWorld PROPERTIES WIN32_EXECUTABLE TRUE)
# 对于 msvc 还需使用定义 WinMain
# #include <windows.h>
# int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
# {
#     return main(__argc, __argv);
# }
# 
```


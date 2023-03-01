---
mytags: myblog
title: Cmake 模板
date: 2023-01-17 21:40:02
categories: [Cmake, 模板]
tags: [Cmake]
---

Cmake 模板
<!-- more -->


```c
cmake_minimum_required(VERSION 3.10)
project(temp)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源文件
file(GLOB_RECURSE SOURCES  
  "src/*.c"
)

# 头文件路径
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

# 特定头文件
file(GLOB_RECURSE INCLUDES 
    
)

# 环境变量
set(LIBS_SYSTEM ws2_32)
set(CMAKE_C_FLAGS "-static" )
set(CMAKE_BUILD_TYPE "Debug")   
# SET(CMAKE_BUILD_TYPE "Release")


# 可执行文件
add_executable(a ${SOURCES} ${INCLUDES})

# 链接库和依赖
# find_package(SDL2 REQUIRED SDL2)
# target_link_libraries(a PRIVATE SDL2::SDL2-static paho-mqtt3c-static ws2_32 winpthread)

# 自定义目标
# add_custom_target (run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main)

```

[Effective Modern CMake](https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1/)



## 假设只有 dll 和 .h 文件的情况下:

FindAWALib.cmake 放在 dll 和 h 同一个文件夹下

```python
get_filename_component(CURRENT_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH) 

# 查找当前 .cmake 所在的文件夹, 若有 awalib.h 文件，则 AWALib_INCLUDE_DIR 变量被设置
# HINTS 后面列出了所有可选的文件夹路径, 用于查找指定的文件
find_path(AWALib_INCLUDE_DIR 
    NAMES awalib.h
    HINTS ${CURRENT_CMAKE_DIR}
)

# 跨平台兼容性
if(WIN32)
    set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .dll)
else()
   set(CMAKE_FIND_LIBRARY_SUFFIXES .a .so)
endif()

find_library(AWALib_LIBRARIES
    NAMES awalib  
    PATHS ${CURRENT_CMAKE_DIR}
)

# 找到头文件和 dll 则创建库 AWALib::AWALib, 并设置 AWALib::AWALib 头文件和 LINK_LIBRARIES 属性
# 方便调用者直接通过 target_link_libraries (xxx AWALib::AWALib) 进行链接, 通过 include_directories(AWALib::AWALib) 进行头文件路径设置
if (AWALib_INCLUDE_DIR AND AWALib_LIBRARIES)
    set(AWALib_FOUND TRUE)

    if(NOT TARGET AWALib::AWALib)
        add_library(AWALib::AWALib INTERFACE IMPORTED)
        set_target_properties(AWALib::AWALib PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${AWALib_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${AWALib_LIBRARIES}")
    endif()
endif ()
```

要使用该 module 其 CMakeLists.txt 如下

```python
cmake_minimum_required(VERSION 3.20)

project(app VERSION 1.0)

# 设置模块查找时搜索的路径
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../awalib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(AWALib REQUIRED MODULE)

include_directories(AWALib::AWALib)
add_executable(demo main.c)
target_link_libraries(demo AWALib::AWALib)

# 构建时自定义, 将 dll 复制到 exe 同路径下，方便直接运行
add_custom_command(TARGET demo POST_BUILD  
    COMMAND ${CMAKE_COMMAND} -E copy ${AWALib_LIBRARIES} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
```

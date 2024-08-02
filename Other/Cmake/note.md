
cmake 和 make 思想类似, 使用目标，比如自定义一个 gen_txt 目标，目标依赖在 DEPENDS 后面

```python
find_program(MYEXE NAMES echo)
message(STATUS "find ${MYEXE}")

# 自定义目标
add_custom_target(gen_txt DEPENDS ${MYEXE}
    COMMAND ${MYEXE} Hello World > Hello.txt
)

# 依赖可以单独写一行进行追加
add_dependencies(gen_txt dep)
```

## 消息打印

```python
# message 打印信息, STATUS 日志类型
message(STATUS "Hello World")
```

## 查找指令

#### 1. find_package

```python
# find_package 用于查找某个包，若存在 PKGConfig.cmake  则 source 该 cmake 文件
# 这时 config.cmake 中定义的变量将在顶层中可以访问

# REQUIRED 是可选参数，用于指定在找不到所需包时报错
find_package(SDL2 REQUIRED)

get_cmake_property(vars VARIABLES)
foreach (v ${vars})
    if (v MATCHES "SDL.*")
        message(STATUS "package var ${v}: ${${v}}")
    endif()
endforeach()

# 找到的 package 有很多的子 lib 或 exe 通过 :: 来访问
target_link_libraries(app SDL2::SDL2main SDL2::SDL2-static)
```

例如编译 Qt 程序

```python
find_package(Qt5 REQUIRED COMPONENTS Core)

# UI 界面 MO 对象 RC 资源自动编译
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
add_executable(app main.cpp Counter.h)
target_link_libraries(app Qt5::Core)
```

#### 2. find_program

```python
# find_program 查找 PATH 下某个可执行程序，找到后可以用来执行命令
find_program(MYEXE NAMES echo)
message(STATUS "find ${MYEXE}")
```

## install 

install 可以把文件复制到某个地方

```python
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Hello.txt DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# 在顶层 add_subdirectory 后会获取子目录的 target 从而直接可以安装子目录的 target
add_subdirectory(add)
install(TARGETS add DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)
```

## 自定义命令

```python
# 自定义命令来复制静态库到指定目录, POST_BUILD: 编译阶段自动执行
# 使用 ${CMAKE_COMMAND} cmake 内置命令，这样有利于跨平台
add_custom_command(TARGET add_static POST_BUILD  
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/lib  
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:add_static> ${CMAKE_CURRENT_SOURCE_DIR}/lib/  
    COMMENT "Copying static libadd to lib directory"  
)
```

## 路径变量

```python
CMAKE_BINARY_DIR 是顶层 build 目录
CMAKE_CURRENT_BINARY_DIR 是自身当前 build 目录, 若是通过 add_subdirectory 引入, 则在顶层 build 目录下的子目录中
CMAKE_CURRENT_SOURCE_DIR 自身当前的源文件目录
CMAKE_CURRENT_LIST_FILE 当前文件路径, 包括 .cmake 文件
# 然后通过以下获取当前文件所在目录
get_filename_component(CURRENT_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH) 

# 设置运行时 dll 或 exe 的输出路径, 需在 add_executable 之前定义有效
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
# 设置静态库的输出路径
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
```

## 目标属性

```python
# 通过设置目标属性可以更改输出目标的名称
add_library(add SHARED add.c)
add_library(add_static STATIC add.c)
set_target_properties(add_static PROPERTIES OUTPUT_NAME add)
```

## 总结

```python
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_BUILD_TYPE "Release")

# TRUE 将程序入口变为 WinMain, 由 WinMain 调用 main
# 这使它成为一个GUI可执行程序，而不是一个控制台应用程序
set(CMAKE_WIN32_EXECUTABLE TRUE)

# 可以对单独某个 target 进行设置
set_target_properties(app PROPERTIES WIN32_EXECUTABLE TRUE)
```

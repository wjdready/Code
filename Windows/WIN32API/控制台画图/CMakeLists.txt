cmake_minimum_required(VERSION 3.10)

project(temp)

add_executable(a main.cpp)
target_link_libraries(a gdi32 serialport)

set(CMAKE_BUILD_TYPE "Release")

# TRUE 为 WIN32 GUI 程序, 不启动控制台, 默认 FALSE
set_target_properties(a PROPERTIES WIN32_EXECUTABLE FALSE)


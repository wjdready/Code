cmake_minimum_required(VERSION 3.10)  

# 若用户不设置 CMAKE_INSTALL_PREFIX 则默认安装到 build 目录下的 add
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/add)

project(addlib)

# 设置运行时和静态库输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

add_library(add SHARED add.c)
add_library(add_static STATIC add.c)
set_target_properties(add_static PROPERTIES OUTPUT_NAME add)

# 安装库和头文件 (有助于在其他项目中使用)
# install(TARGETS add 
#     ARCHIVE DESTINATION lib  
#     LIBRARY DESTINATION lib  
#     RUNTIME DESTINATION bin
# )

# install(FILES include/add.h  
#     DESTINATION include/addlib  
# )

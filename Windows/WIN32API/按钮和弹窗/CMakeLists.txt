cmake_minimum_required(VERSION 3.28)

project(test)

add_executable(a test.c)

target_link_libraries(a PRIVATE Comdlg32)

# TRUE 为 WIN32 GUI 程序, 不启动控制台, 默认 FALSE
set_target_properties(a PROPERTIES WIN32_EXECUTABLE TRUE)

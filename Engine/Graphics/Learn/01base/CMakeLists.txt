cmake_minimum_required(VERSION 3.10)

# 设置 vcpkg 工具链文件路径和目标三元组即可使用 vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/ProgramFiles/Library/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x64-windows-static")

if(MSVC)
    add_compile_options(/utf-8)
endif()

# if(WIN32)
#     set(CMAKE_GENERATOR_PLATFORM Win32 CACHE STRING "Platform" FORCE)
# endif()

project(HelloWorld)

set(CMAKE_BUILD_TYPE "Release")

find_package(imgui CONFIG  REQUIRED)
find_package(glfw3 CONFIG  REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
    src/common
    src/draw
)

file(GLOB_RECURSE SOURCES  
  src/demos/*.cpp
  src/common/*.cpp
  src/draw/*.cpp
  src/main.cpp
)

add_executable(HelloWorld ${SOURCES})

target_link_libraries(HelloWorld PRIVATE 
    imgui::imgui
    glfw
    ${OPENGL_LIBRARIES}
)

# 添加运行目标
add_custom_target(run
    COMMAND $<TARGET_FILE:HelloWorld>
    DEPENDS HelloWorld
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running HelloWorld"
)

# TRUE 为 WIN32 GUI 程序, 不启动控制台, 默认 FALSE
set_target_properties(HelloWorld PROPERTIES WIN32_EXECUTABLE FALSE)

# 添加链接选项，排除冲突的默认库
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
endif()

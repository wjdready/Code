cmake_minimum_required(VERSION 3.20)

project(app VERSION 1.0)

set(CMAKE_BUILD_TYPE "Debug")

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)
set(QT "Qt${QT_VERSION_MAJOR}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE SOURCES  
    "src/*.cpp"
    "resource/resource.qrc"
)

list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "resource/ui")

add_executable(app ${SOURCES})

target_link_libraries(app ${QT}::Core ${QT}::Widgets)

# TRUE 为 WIN32 GUI 程序, 不启动控制台, 默认 FALSE
set_target_properties(app PROPERTIES WIN32_EXECUTABLE FALSE)

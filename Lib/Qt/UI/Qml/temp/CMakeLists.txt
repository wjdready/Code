cmake_minimum_required(VERSION 3.20)

project(app VERSION 1.0)

set(CMAKE_BUILD_TYPE "Debug")

find_package(Qt6 REQUIRED COMPONENTS Core Quick)
set(QT "Qt${QT_VERSION_MAJOR}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE SOURCES  
    "src/*.cpp"
    "resource/res.qrc"
)

add_executable(app ${SOURCES})

target_link_libraries(app ${QT}::Core ${QT}::Quick)

# ========== 使用 Felgo ==============
set(USE_FELGO_LIVE ON)
if(USE_FELGO_LIVE)
    add_definitions(-DUSE_FELGO_LIVE=1)
    find_package(FelgoLive REQUIRED)
    find_package(Felgo REQUIRED)
    target_link_libraries(app FelgoLive Felgo)
endif()  

# TRUE 为 WIN32 GUI 程序, 不启动控制台, 默认 FALSE
set_target_properties(app PROPERTIES WIN32_EXECUTABLE FALSE)

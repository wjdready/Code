cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE "C:/ProgramFiles/Library/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x86-windows-static")

project(app VERSION 1.0)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick QmlImportScanner)
set(QT "Qt${QT_VERSION_MAJOR}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_BUILD_TYPE "Release")

file(GLOB_RECURSE SOURCES  
    "src/*.cpp"
    "res/res.qrc"
)

add_executable(app ${SOURCES})

set_property(TARGET app PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(app PRIVATE ${QT}::Core ${QT}::Quick)

find_package(Qt5 COMPONENTS QmlImportScanner)
qt5_import_qml_plugins(app)

# ========== 使用 Felgo ==============
set(USE_FELGO_LIVE OFF)
if(USE_FELGO_LIVE)
    add_definitions(-DUSE_FELGO_LIVE=1)
    find_package(FelgoLive REQUIRED)
    find_package(Felgo REQUIRED)
    target_link_libraries(app FelgoLive Felgo)
endif()  

# TRUE 为 WIN32 GUI 程序, 不启动控制台, 默认 FALSE
set_target_properties(app PROPERTIES WIN32_EXECUTABLE TRUE)



CMake 编译生成 win32 平台

cmake -B build -G "Visual Studio 17 2022" -A Win32

生成 Release 版本
cmake --build build --config Release


## 构建

```sh
vcpkg search imgui
# 安装 imgui 库，包含 opengl3-binding 和 glfw-binding
vcpkg install imgui[opengl3-binding,glfw-binding]:x86-windows-static

cmake -B build
cmake --build build --config Release --target run
```

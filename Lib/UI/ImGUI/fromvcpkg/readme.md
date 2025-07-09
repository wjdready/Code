```sh
# 查找
vcpkg search imgui
# 安装 imgui 库，包含 opengl3-binding 和 glfw-binding
vcpkg install imgui[opengl3-binding,glfw-binding]:x86-windows-static
```


CMakeLists.txt

```python
cmake_minimum_required(VERSION 3.10)

# 设置 vcpkg 工具链文件路径和目标三元组即可使用 vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/ProgramFiles/Library/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET "x86-windows-static")

if(WIN32)
    set(CMAKE_GENERATOR_PLATFORM Win32 CACHE STRING "Platform" FORCE)
endif()

project(HelloWorld)

set(CMAKE_BUILD_TYPE "Release")

find_package(imgui CONFIG  REQUIRED)
find_package(glfw3 CONFIG  REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(HelloWorld imgui.cpp)

target_link_libraries(HelloWorld PRIVATE 
    imgui::imgui
    glfw
    ${OPENGL_LIBRARIES}
)

# TRUE 为 WIN32 GUI 程序, 不启动控制台, 默认 FALSE
set_target_properties(HelloWorld PROPERTIES WIN32_EXECUTABLE TRUE)

# 添加链接选项，排除冲突的默认库
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
endif()
```

imgui.cpp

```cpp
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

int main(int, char**)
{
    // 初始化 GLFW
    if (!glfwInit())
        return 1;

    // 创建 GLFW 窗口
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui Example", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // 设置 ImGui 上下文
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // 设置 ImGui 样式
    ImGui::StyleColorsLight();

    // 加载字体
    io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    // 初始化 ImGui 后端
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // 主循环
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // 开始新的 ImGui 帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 创建一个简单的 ImGui 窗口
        ImGui::Begin("Hello, ImGui!");
        ImGui::Text("This is a simple ImGui example.");
        if (ImGui::Button("点我 Click Me!"))
        {
            printf("Button clicked!\n");
        }
        ImGui::End();

        // 渲染 ImGui
        ImGui::Render();
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // 清理资源
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    return main(__argc, __argv);
}
```
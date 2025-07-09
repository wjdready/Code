#include "common.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

void drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    ImDrawList *dl = ImGui::GetBackgroundDrawList();
    dl->AddCircleFilled(ImVec2((float)x, (float)y), 1.0f, ImColor(r, g, b, a));
}

float backgroundColor[4] = {0.0f, 0.0f, 0.0f, 1.0f}; // RGBA

void draw_update()
{
    ImGui::Begin("示例");

    if (ImGui::TreeNode("全局设置"))
    {
        ImGui::ColorEdit4("背景颜色", backgroundColor);

        ImGui::TreePop();
    }

    for (int i = 0; i < (int)demoFunctions.size(); ++i)
    {
        demoFunctions[i]();
    }

    ImGui::End();
}

int main(int, char **)
{
    // 初始化 GLFW
    if (!glfwInit())
        return 1;

    // 创建 GLFW 窗口
    GLFWwindow *window = glfwCreateWindow(1280, 720, "ImGui Example", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // 设置 ImGui 上下文
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // 加载字体
    io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 18.0f, NULL,
                                 io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    // 设置 ImGui 样式
    ImGui::StyleColorsLight();

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

        // 绘制示例
        draw_update();

        // 渲染 ImGui
        ImGui::Render();
        glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
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

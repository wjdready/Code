#include "common.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <vector>

// 定义纹理 ID 类型
typedef unsigned int GLuint;

// 纹理对象
GLuint textureID = 0;
// 纹理宽度和高度
const int textureWidth = 400;
const int textureHeight = 400;
// 像素数据
std::vector<uint8_t> pixelData(textureWidth * textureHeight * 4);

// 初始化纹理
void initTexture() {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 上传像素数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData.data());
    glBindTexture(GL_TEXTURE_2D, 0);
}

// 更新像素数据
void updatePixelData() {
    // 这里可以根据需求更新像素数据
    // for (int y = 0; y < textureHeight; ++y) {
    //     for (int x = 0; x < textureWidth; ++x) {
    //         int index = (y * textureWidth + x) * 4;
    //         pixelData[index] = static_cast<uint8_t>(x % 256);     // R
    //         pixelData[index + 1] = static_cast<uint8_t>(y % 256); // G
    //         pixelData[index + 2] = 128;                           // B
    //         pixelData[index + 3] = 255;                           // A
    //     }
    // }
    // 更新纹理数据
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, textureWidth, textureHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixelData.data());
    glBindTexture(GL_TEXTURE_2D, 0);
}

// 原有的 drawPixel 函数，可保留或注释掉
// void drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
// {
//     ImDrawList *dl = ImGui::GetBackgroundDrawList();
//     dl->AddCircleFilled(ImVec2(x, y), 1.0f, ImColor(r, g, b, a));
// }

void drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    // 检查坐标是否在有效范围内
    if (x >= 0 && x < textureWidth && y >= 0 && y < textureHeight) {
        int index = (y * textureWidth + x) * 4;
        pixelData[index] = r;         // R
        pixelData[index + 1] = g;     // G
        pixelData[index + 2] = b;     // B
        pixelData[index + 3] = a;     // A
    }
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

    // 绘制纹理
    if (textureID) {
        ImVec2 size(textureWidth, textureHeight);
        ImVec2 uv0(0.0f, 0.0f);
        ImVec2 uv1(1.0f, 1.0f);
        ImColor tint_col(255, 255, 255, 255);
        ImColor border_col(255, 255, 255, 128);
        // ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(textureID)), size, uv0, uv1, tint_col, border_col);
        ImGui::Image(textureID, size, uv0, uv1, tint_col, border_col);
    }

    for (int i = 0; i < demoFunctions.size(); ++i)
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

    // 初始化纹理
    initTexture();

    // 主循环
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // 开始新的 ImGui 帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 更新像素数据
        updatePixelData();

        // 绘制示例
        draw_update();

        // 渲染 ImGui
        ImGui::Render();
        glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // 清理纹理
    if (textureID) {
        glDeleteTextures(1, &textureID);
    }

    // 清理资源
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

// #include <windows.h>
// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
// {
//     return main(__argc, __argv);
// }
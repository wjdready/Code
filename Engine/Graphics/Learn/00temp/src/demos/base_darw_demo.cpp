#include "common.h"
#include "draw.h"

static void baseDrawDemo(void)
{
    static int circleX = 400;
    static int circleY = 300;
    static float circleColor[4] = {0.0f, 1.0f, 0.0f, 1.0f}; // 初始颜色为绿色
    static int rad = 50;
    static bool isDrawShow = false;

    // 创建可折叠列表
    if (ImGui::TreeNode("绘制圆形"))
    {
        ImGui::Checkbox("显示圆形", &isDrawShow);

        // 添加滑动条控制圆的 X 坐标
        ImGui::SliderInt("Circle X", &circleX, 0, 800);
        // 添加滑动条控制圆的 Y 坐标
        ImGui::SliderInt("Circle Y", &circleY, 0, 600);
        // 添加颜色控件控制圆的颜色
        ImGui::ColorEdit4("Circle Color", circleColor);
        // 添加滑动条控制圆的半径
        ImGui::SliderInt("Circle Radius", &rad, 1, 100);

        // 结束可折叠列表
        ImGui::TreePop();
    }

    if (isDrawShow)
    {
        // 将颜色值从 float [0, 1] 转换为 uint8_t [0, 255]
        uint8_t r = static_cast<uint8_t>(circleColor[0] * 255);
        uint8_t g = static_cast<uint8_t>(circleColor[1] * 255);
        uint8_t b = static_cast<uint8_t>(circleColor[2] * 255);
        uint8_t a = static_cast<uint8_t>(circleColor[3] * 255);
        drawCircle(circleX, circleY, rad, r, g, b, a);
    }
}
REGISTER_DEMO(baseDrawDemo);

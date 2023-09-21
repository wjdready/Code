#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#include <SDL.h>
#include <SDL_opengles2.h>
#include <stdio.h>

void main_windows(void)
{
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("中文你好世界");

    ImGui::Text("这是一些有用的文本 %f", f);
    ImGui::SliderFloat("浮点数", &f, 0.0f, 1.0f);

    if (ImGui::Button("按键"))
        counter++;
    ImGui::SameLine();
    ImGui::Text("计数 = %d", counter);

    ImGui::End();
}

void quick_demo(void)
{
    static bool selected[10] = {};

    if (ImGui::BeginTable("split1", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
    {
        for (int i = 0; i < 10; i++)
        {
            char label[32];
            sprintf(label, "Item %d", i);
            ImGui::TableNextColumn();
            ImGui::Selectable(label, &selected[i]);
        }
        ImGui::EndTable();
    }
}

void animate_sin(void)
{
    static float values[90] = {};
    static int values_offset = 0;
    static double refresh_time = 0.0;
    static bool animate = true;
    if (!animate || refresh_time == 0.0)
        refresh_time = ImGui::GetTime();
    while (refresh_time < ImGui::GetTime()) // Create data at fixed 60 Hz rate for the demo
    {
        static float phase = 0.0f;
        values[values_offset] = cosf(phase);
        values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
        phase += 0.10f * values_offset;
        refresh_time += 1.0f / 60.0f;
    }

    // Plots can display overlay texts
    // (in this example, we will display an average value)
    {
        float average = 0.0f;
        for (int n = 0; n < IM_ARRAYSIZE(values); n++)
            average += values[n];
        average /= (float)IM_ARRAYSIZE(values);
        char overlay[32];
        sprintf(overlay, "avg %f", average);
        ImGui::GetStyle().Colors[ImGuiCol_PlotLines] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, overlay, -1.0f, 1.0f, ImVec2(0, 80.0f));
    }
}

void ui_update(void)
{
    // ImGui::ShowDemoWindow(true);

    main_windows();

    quick_demo();

    animate_sin();

    // 背景色
    ImVec4 bg = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glClearColor(bg.x * bg.w, bg.y * bg.w, bg.z * bg.w, bg.w);
}

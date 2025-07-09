#ifndef __COMMON_H__
#define __COMMON_H__
#include "imgui.h"
#include <cstdint>
#include <functional>
#include <vector>

extern float backgroundColor[4];
void drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/////////////////// Demo 自动注册 //////////////////////////
// 定义函数类型
using DemoFunction = std::function<void()>;

// 全局的函数数组
extern std::vector<DemoFunction> demoFunctions;

// 注册器类
class DemoRegistrar
{
  public:
    DemoRegistrar(DemoFunction func)
    {
        demoFunctions.push_back(func);
    }
};

// 注册宏
#define REGISTER_DEMO(func) static DemoRegistrar registrar_##func(func)

#endif

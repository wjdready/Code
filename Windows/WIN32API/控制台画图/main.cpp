#include <vector>
#include <windows.h>

// 使用std::vector来存储点
std::vector<POINT> points;

// 窗口过程函数声明
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

// 窗口类名
#define WINDOW_CLASS_NAME "Sample Window Class"

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
    HWND hwnd;        // 窗口句柄
    MSG messages;     // 消息
    WNDCLASSEX wincl; // 窗口类

    // 窗口类结构
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = WINDOW_CLASS_NAME;
    wincl.lpfnWndProc = WindowProcedure; // 窗口过程
    wincl.style = CS_DBLCLKS;            // 双击
    wincl.cbSize = sizeof(WNDCLASSEX);

    // 默认图标和鼠标指针
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                      // 无菜单
    wincl.cbClsExtra = 0;                           // 没有额外窗口数据
    wincl.cbWndExtra = 0;                           // 没有额外窗口数据
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND; // 窗口背景颜色

    if (!RegisterClassEx(&wincl))
        return 0;

    // 创建窗口
    hwnd = CreateWindowEx(0,                   // 扩展风格
                          WINDOW_CLASS_NAME,   // 类名
                          "Learn WinAPI",      // 窗口标题
                          WS_OVERLAPPEDWINDOW, // 窗口风格
                          CW_USEDEFAULT,       // 窗口位置X
                          CW_USEDEFAULT,       // 窗口位置Y
                          544,                 // 窗口宽度
                          375,                 // 窗口高度
                          HWND_DESKTOP,        // 父窗口句柄
                          NULL,                // 无菜单句柄
                          hThisInstance,       // 程序实例句柄
                          NULL                 // 附加的应用程序数据
    );

    ShowWindow(hwnd, nCmdShow);

    // 消息循环
    while (GetMessage(&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

// 窗口过程函数
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        // 遍历所有点并重新绘制它们
        for (const auto &pt : points)
        {
            SetPixel(hdc, pt.x, pt.y, RGB(255, 0, 0));
        }

        EndPaint(hwnd, &ps);
        break;

    case WM_LBUTTONDOWN:
    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON)
        { // 检查鼠标左键是否被按下
            POINTS pt = MAKEPOINTS(lParam);
            points.push_back({pt.x, pt.y}); // 将新点添加到点数组中

            hdc = GetDC(hwnd);                         // 获取设备上下文进行绘制
            SetPixel(hdc, pt.x, pt.y, RGB(255, 0, 0)); // 绘制当前点
            ReleaseDC(hwnd, hdc);                      // 释放设备上下文

            // 不需要InvalidateRect，因为WM_PAINT会在下一次窗口重绘时调用
        }
        break;

    case WM_LBUTTONUP:
        // 可以在这里处理鼠标释放事件，但不需要清除点
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

#include <windows.h>
#include <stdio.h>

// Window procedure function
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hButton;

    switch (message)
    {
    case WM_CREATE:
        // Create the button and store its handle
        hButton = CreateWindow("Button", "Click Me", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 100, 50, hwnd,
                               (HMENU)1, GetModuleHandle(NULL), NULL);

        CreateWindow("Button", "Open File", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 100, 50, hwnd, (HMENU)2,
                     GetModuleHandle(NULL), NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) // Button ID is 1
        {
            MessageBox(hwnd, TEXT("Button Clicked!"), TEXT("Info"), MB_OK);
        }
        else if (LOWORD(wParam) == 2)
        {
            OPENFILENAME ofn; // 文件名结构体
            char szFile[260]; // 文件名缓冲区

            // 初始化结构体
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = szFile;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = "All File (*.*)\0*.*\0Text File (*.txt)\0*.txt\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            // 显示对话框
            if (GetOpenFileName(&ofn))
            {
                MessageBox(hwnd, ofn.lpstrFile, "Select File", MB_OK);
            }
        }
        break;

    case WM_SIZE: {
        // Get the new client size
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);

        // Calculate the new position for the button to center it
        int buttonWidth = 100;
        int buttonHeight = 50;
        SetWindowPos(hButton, NULL, (width - buttonWidth) / 2, (height - buttonHeight) / 2, buttonWidth, buttonHeight,
                     SWP_NOZORDER); // Retain the Z order
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    AllocConsole();
    FILE* file;
    freopen_s(&file, "CONOUT$", "w", stdout); // Redirect stdout to console
    freopen_s(&file, "CONOUT$", "w", stderr); // Redirect stderr to console
    freopen_s(&file, "CONIN$", "r", stdin);   // Redirect stdin to console

    WNDCLASS wc = {0};
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("MyWindowClass");
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClass(&wc))
        return 0;

    HWND hwnd = CreateWindow(wc.lpszClassName, TEXT("My Window"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                             300, 200, NULL, NULL, hInstance, NULL);
    if (!hwnd)
        return 0;

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

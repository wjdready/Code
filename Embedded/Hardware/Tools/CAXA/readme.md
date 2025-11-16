
## 二次开发

使用的是 C++ 开发

安装CAXA 后，`菜单栏->帮助->二次开发资源` 获得开发帮助和开发资源, 可以看到提供的 `CRX\Inc` 和 `CRX\Lib` 

然后还有 ObjectCRX_Samples 是一些示例

### 1. 开发环境

根据 `Doc\ObjectCRX_Main.chm` 开发依赖 VS2019

现在官网找不到安装 vs_community 的版本, 可从这里[下载](https://www.ddooo.com/softdown/142446.htm)

然后下载 C++ 开发环境和 MFC 库，`Visual Studio Installer` 选择 `单个组件`, 搜索 MFC 选择最下面的最新即可

安装 `CRX\Wizard` 向导工具，这个是用来创建crx项目的向导工具程序, 它会在创建 VS 项目时自动完成一些库和头文件依赖等配置

设置环境变量 `CRX_PATH_2023_x64` 为安装目录下的 `CRX` 目录, 比如 `C:\ProgramFiles\Caxa\CRX` 从而在创建 crx 项目时自动导入库和头文件

## 2. 使用 VSCode 开发

使用 file 查看生成的 crx 发现其时 crx 是一个 DLL 文件, 加载命令时其实是加载 dll 并执行其中指定的函数

同时实验表面不一定需要 VS2019 和安装 Wizard, 拿到 Lib 和 Inc 就其实可以开发了

因此 CMake 如下:

```sh
cmake_minimum_required(VERSION 3.10)

project(CreateArc)

# 启用 MFC 支持
set(CMAKE_MFC_FLAG 2)

add_definitions(-DUNICODE)

# 配置 Caxa CRX 路径:
set(CAXA_CRX_PATH "C:/ProgramFiles/Caxa/CRX")

File(GLOB_RECURSE SRC_FILES 
    "*.cpp"
)

include_directories(${CAXA_CRX_PATH}/Inc)

add_library(${PROJECT_NAME} SHARED
    ${SRC_FILES}
)

# 首先设置库文件搜索路径
target_link_directories(${PROJECT_NAME} PRIVATE ${CAXA_CRX_PATH}/Lib)

target_link_libraries(${PROJECT_NAME} PRIVATE
    mfc140u.lib
    Crx.lib
    CrxDb.lib
    CrxEdApi.lib
    CrxGe.lib
    CrxGi.lib
    CrxSpt.lib
    CrxUi.lib
)

# 设置输出文件扩展名为 .crx
set_target_properties(${PROJECT_NAME} PROPERTIES
    SUFFIX ".crx"
    OUTPUT_NAME "${PROJECT_NAME}"
)

# cmake -B build -G "Visual Studio 17 2022"
# cmake --build build --config Release
```

### 3. 使用 crx 模块

`菜单栏->加载应用->模块管理->指定文件` 

加载后在图纸编辑界面直接输入自定义命令, 比如 `LCreateArc` 即可

命令行窗口可以显示命令输出和输入历史, 在编辑界面左侧栏空白处右击，弹出菜单选择 `命令行` 即可显示命令行窗口

## 示例

### 1. Hello

```cpp
// MyHello.cpp

// ---------- MFC -----------------
#include <afxwin.h>

// ------ CAXA editor API ---------
#include "CRxDefine.h"
#include "crxcmd.h"
#include "crxutcds.h"
#include "rxdict.h"
#include "crxEntryPoint.h"
#include "CRxExtensionModule.h"

AC_IMPLEMENT_EXTENSION_MODULE(Hello1DLL)

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _hdllInstance = hInstance;
        Hello1DLL.AttachInstance(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        Hello1DLL.DetachInstance();
    }
    return (TRUE);
}

void cmdHello()
{
    crxutPrintf(_T("\nHello World!"));
}

class CHello1App : public AcRxArxApp
{
  public:
    CHello1App() : AcRxArxApp()
    {
    }

    virtual AcRx::AppRetCode On_kInitAppMsg(void *pkt)
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

        crxedRegCmds->addCommand(_T("HelloApp"), _T("GHello"), _T("MyHello"), ACRX_CMD_MODAL, &cmdHello);
        return (retCode);
    }

    virtual AcRx::AppRetCode On_kUnloadAppMsg(void *pkt)
    {
        AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

        crxedRegCmds->removeGroup(_T("HelloApp"));
        return (retCode);
    }

    virtual void RegisterServerComponents()
    {
    }
};

IMPLEMENT_ARX_ENTRYPOINT(CHello1App)
```

使用上面的 CMakeLists.txt 设置 `project(MyHello)` 生成 MyHello.crx, 然后加载到 CAXA, 在 CAXA 命令行中输入 `MyHello` 将看到 `Hello World!`

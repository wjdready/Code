
## Felgo
Felgo 支持 QML 热更新

使用时只需在 CMake 中添加:

```python
# ========== 使用 Felgo ==============
set(USE_FELGO_LIVE ON)
if(USE_FELGO_LIVE)
    add_definitions(-DUSE_FELGO_LIVE=1)
    find_package(FelgoLive REQUIRED)
    find_package(Felgo REQUIRED)
    target_link_libraries(app FelgoLive Felgo)
endif()  
```

在 main.cpp 中添加

```C
#if USE_FELGO_LIVE
#include <FelgoLiveClient>
#include <FelgoApplication>
#endif

int main()
{
    // ...
#if USE_FELGO_LIVE
    FelgoApplication felgo;
    felgo.initialize(&engine);
    // felgo.setMainQmlFileName(QStringLiteral("qrc:/qml/main.qml"));
    // engine.load(QUrl(felgo.mainQmlFileName()));
    FelgoLiveClient liveClient(&engine);
#else
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
#endif

}
```

## 开源库

kirigami -- KDE 之作
fluid    -- Material Design 风格
FluentUI -- FluentUI 风格



## 自定义模块

参考 qt-everywhere-src-6.6.0\qtdeclarative\examples\quickcontrols\wearable\Wearable

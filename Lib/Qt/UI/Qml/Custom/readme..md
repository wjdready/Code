

关于自定义的官方教程: https://doc.qt.io/qt-6/qtquickcontrols-customize.html#customization-reference

Controls 官方文档: https://doc.qt.io/qt-6/qtquickcontrols-index.html

# Style

官方教程: https://doc.qt.io/qt-6/qtquickcontrols-styles.html

## MSYS 环境中也是有 style 的

```sh
# C:\ProgramFiles\msys64\ucrt64\share\qt6\qml\QtQuick\Controls
# 通过以下方式可找到相关包, 实际已经安装
pacman -Fx .*qt6.*material.*

# 然后直接使用该风格
./app -style Material
```


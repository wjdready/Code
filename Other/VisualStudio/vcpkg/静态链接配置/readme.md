

参考工程:
https://github.com/RibShark/SafeDiscShim


注意:
set(VCPKG_TARGET_TRIPLET "x86-windows-static")
需要放在最前面才有效

同时还要配合 VS 管理配置，添加一个 x86-Release 的配置

qml 的静态链接需要追加图下内容
https://stackoverflow.com/questions/13591951/module-qtquick-plugin-qtquick2plugin-not-found-in-static-linked-application
https://doc.qt.io/qt-5/qtqml-cmake-qt5-import-qml-plugins.html


Qt 静态链接 32bit:
最小例子: 基本界面 6.4MB, 加 qml 10.8M
Qt5.12 32bit 动态库打包压缩: 7.64MB, 加 qml 11.8MB

而 flutter 最小例子压缩后 11.0MB

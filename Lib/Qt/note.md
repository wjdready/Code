

* 禁止一些警告 提示的是 -Wunused-function, -W 后加 no
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-function

## QT API 

https://felgo.com/doc/felgo-qt-overview/


# 布局心得

#### 布局方式无法选择?

选布局方式时, 里面至少包含一个控件才可点击布局方式

Ubuntu 编译 Qt 缺失 qml 

```sh
apt-get install qtdeclarative5-dev
```

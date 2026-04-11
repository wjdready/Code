

```cpp
// testview.h
#pragma once
#include <QWidget>

class TestView : public QWidget
{
    Q_OBJECT

  public:
    TestView();
};

// testview.cpp
#include "testview.h"
#include <QQuickWidget>
#include <QVBoxLayout>
#include <QLineEdit>

TestView::TestView()
{
    setMinimumSize(720, 480);

    // 创建上下布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QLineEdit *inputLine = new QLineEdit(this);
    mainLayout->addWidget(inputLine);

    QQuickWidget *quickwidget = new QQuickWidget();
    quickwidget->setSource(QUrl("qrc:/qml/keyboard.qml"));
    quickwidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    quickwidget->setFocusPolicy(Qt::NoFocus);
    mainLayout->addWidget(quickwidget);
}

// main.cpp
#include <QApplication>
#include "testview.h"

int main(int argc, char *argv[])
{
    // 设置输入法模块为Qt虚拟键盘, 设置输入法主题, 可选 default retro
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    qputenv("QT_VIRTUALKEYBOARD_STYLE", QByteArray("default"));

    QApplication app(argc, argv);

    // 1. 退出崩溃
    // 估计是因为 static 在程序最后阶段才退出, 导致app销毁后, quickwidget 还在渲染, 从而引发
    // QQuickWidget::invalidateRenderControl could not make context current 
    static TestView testView;
    testView.show();

    // 2. 退出正常
    // TestView testView;
    // testView.show();

    // 3. 退出正常, 在末尾 delete
    // TestView *testView = new TestView();
    // testView->show();

    int ret = app.exec();

    // delete testView;

    return ret;
}
```

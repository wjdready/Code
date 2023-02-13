#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "AWAJsonSerial.h"
#include <QMutex>
#include <QWidget>

class QSerialPort;

namespace Ui {
    class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

public slots:
    // 通用槽函数
    void openSerial();              // 打开串口
    void closeSerial();             // 关闭串口
    void refreshSerial();           // 刷新串口
    void sendData();                // 发送数据

    // 测试按钮的点击事件槽函数
    void connectTest();             // 连接测试
    void getAuthInfoTest();         // 获取授权信息测试
    void getCaliInfoTest();         // 获取校准信息测试
    void getFileListTest();         // 获取文件列表测试

    // 接收 AWAJson 处理结果的槽函数
    void AWAJsonHandleResult(int type, QByteArray *data);

signals:
    void AWAJsonHandle(int type, int timeout);
    void AWAJsonSendRawData(const QByteArray &data, int timeout);

private:
    Ui::MainWidget *ui;
    AWAJsonSerial *AWAJson;
    QSerialPort *serial;
};

#endif

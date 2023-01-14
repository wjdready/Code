#pragma once

#include <QtCore/QObject>
#include <QtSerialPort/QSerialPort>

class MyClass : public QObject
{
    Q_OBJECT

public:
    MyClass() {}
    void start(void);

public slots:
    void recvData(void);

private:
    QSerialPort *serial;
};

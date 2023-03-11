#pragma once

#include <QtCore/QObject>
#include <QtSerialPort/QSerialPort>

class MyClass : public QObject
{
    Q_OBJECT

public:
    MyClass() {}
    void start(void);

private:
    QSerialPort *serial;
};

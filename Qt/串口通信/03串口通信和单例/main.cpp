#include <QtWidgets/QApplication>
#include <QtCore/QDebug>
#include "MyClass.h"
#include <QElapsedTimer>
#include <QThread>
#include "SerialPort.h"

void MyClass::start(void)
{
    serial = SerialPort::getSerial();
    serial->setBaudRate(115200);
    serial->setPortName("COM21");
    if (!serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "Open COM21 FAILED!";
        return;
    }
}

void MyClass::test(void)
{
    serial->write("<N>{\"Class\":\"101\",\"Flag\":\"20230207125348000\"}</N>");
    
    if(serial->waitForReadyRead(10))
    {
        qDebug() << "Read is " << serial->readAll();
    }
    else
    {
        qDebug() << "Timeout";
    }

    serial->close();

    // SerialPort::getSInstance().serialSendRecvTest();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MyClass m;
    m.start();
    m.test();
    return app.exec();
}

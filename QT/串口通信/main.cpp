#include <QtWidgets/QApplication>
#include <QtCore/QDebug>
#include "MyClass.h"

void MyClass::recvData(void)
{
    if (serial->bytesAvailable())
    {
        const QByteArray dat = serial->readAll();
        qDebug() << "已接收：" << QString::fromUtf8(dat);
    }
}

void MyClass::start(void)
{
    serial = new QSerialPort();
    connect(serial, QSerialPort::readyRead, this, recvData);

    serial->setPortName("COM36");
    serial->setBaudRate(115200);

    if (serial->open(QIODevice::ReadWrite))
        qDebug() << "serial open OK ";
    else
        qDebug() << "serial open ERROR ";
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MyClass m;
    m.start();

    return app.exec();
}

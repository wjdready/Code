#include "SerialPort.h"
#include <QThread>
#include <QDebug>

SerialPort::SerialPort()
{
    QThread *thread = new QThread;

    this->moveToThread(thread);
    connect(thread, QThread::started, this, serialThreadStart);
    connect(this, finished, thread, QThread::quit);

    thread->start();
}

SerialPort &SerialPort::getSInstance()
{
    static SerialPort singleInstance;
    return singleInstance;
}

QSerialPort *SerialPort::getSerial()
{
    return &(getSInstance().serial);
}

void SerialPort::serialSendRecvTest()
{
    qDebug() << "Now call from other thread";

    if(serial.isOpen() == false)
    {
        serial.setBaudRate(115200);
        serial.setPortName("COM21");
        if (!serial.open(QIODevice::ReadWrite))
        {
            qDebug() << "Open COM21 FAILED!";
            return;
        }
    }
    
    serial.write("<N>{\"Class\":\"101\",\"Flag\":\"20230207125348000\"}</N>");
    
    if(serial.waitForReadyRead(10))
    {
        qDebug() << "Read is " << serial.readAll();
    }
    else
    {
        qDebug() << "Timeout";
    }

    serial.close();

    qDebug() << "DONE ";
}

void SerialPort::serialThreadStart()
{
    while (1)
    {
        QThread::usleep(100000);
        // qDebug() << "Running...";
    }
}


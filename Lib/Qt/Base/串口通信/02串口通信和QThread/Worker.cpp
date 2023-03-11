#include "Worker.h"
#include <QSerialPort>
#include <QElapsedTimer>
#include <QDebug>
#include <QThread>

Worker::Worker()
{
}


Worker::~Worker()
{
}

void Worker::process() 
{
    QSerialPort serial;
    serial.setPortName("COM21");
    serial.setBaudRate(115200);

    if (!serial.open(QIODevice::ReadWrite))
    {
        qDebug() << "Open COM21 FAILED!";
        return;
    }

    QElapsedTimer timer;
    timer.start();
    int read = 0;

    while (1)
    {
        QThread::usleep(100000);
        serial.write("<N>{\"Class\":\"101\",\"Flag\":\"20230207125348000\"}</N>");
        if(serial.waitForReadyRead(10))
        {
            qDebug() << "Read is " << serial.readAll();
        }
        else 
        {
            qDebug() << "Timeout";
        }
    }
    
    if(read == 0)
        qDebug() << "Timeout to read";

    serial.close();

    emit finished();
    qDebug() << "DONE ";
}


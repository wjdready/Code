#pragma once

#include <QSerialPort>
#include <QThread>
#include <QByteArray>
#include <QMutex>

class SerialPort : public QObject
{
    Q_OBJECT

public:
    static SerialPort &getSInstance();
    static QSerialPort *getSerial();
    void serialSendRecvTest();

signals:
    void finished();

private slots:
    void serialThreadStart();

private:
    SerialPort();
    QSerialPort serial;
};


#pragma once

#include <QtCore/QObject>

class Worker : public QObject
{
    Q_OBJECT

public:
    Worker();
    ~Worker();

public slots:
    void process();

signals:
    void valueChanged(int newValue);
    void finished();
    void error(QString err);
};

#pragma once
#include <QObject>

class MyClass : public QObject
{
    Q_OBJECT

public:
    void startTest();
    void startTest2();

public slots:
    void process();

signals:
    void finished();

};
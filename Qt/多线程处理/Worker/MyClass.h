#pragma once
#include <QObject>

class MyClass : public QObject
{
    Q_OBJECT

public:
    void startTest();
};
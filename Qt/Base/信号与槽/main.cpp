#include "Counter.h"
#include <QtCore/QDebug>

void Counter::setValue(int value)
{
    if (value != m_value)
    {
        m_value = value;
        emit valueChanged(value);
    }
}

int main(void)
{
    Counter a, b;
    QObject::connect(&a, SIGNAL(valueChanged(int)), &b, SLOT(setValue(int)));

    a.setValue(12); // a.value() == 12, b.value() == 12
    b.setValue(48); // a.value() == 12, b.value() == 48
    
    qDebug() << "a value " << a.value();
    qDebug() << "b value " << b.value();

    getchar();
    
    return 0;
}

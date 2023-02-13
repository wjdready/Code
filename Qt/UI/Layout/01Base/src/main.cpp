#include "MainWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    
    // w.setMinimumSize(1080, 720);

    w.show();

    return a.exec();
}

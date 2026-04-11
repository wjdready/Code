#include <QApplication>
#include <QDebug>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <stdio.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget *w = new QWidget();
    w->resize(400, 300);
    w->show();

    new QPushButton("leak");

    QVBoxLayout *mainLayout = new QVBoxLayout(w);
    QPushButton *btn = new QPushButton("Test Button");
    mainLayout->addWidget(btn);

    QObject::connect(btn, &QPushButton::clicked, w, [w]() { qApp->quit(); });

    int ret = app.exec();

    // int *a = new int(0);
    // printf("%d\n", *a);

    // delete w;

    return ret;
}


// valgrind --leak-check=full --show-leak-kinds=definite ./main 

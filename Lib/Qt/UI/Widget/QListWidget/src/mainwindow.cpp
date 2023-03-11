#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QElapsedTimer>
#include <QScrollBar>

#include "ListItem.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    qDebug() << "MainWindow start";

    QFile file(":/style/style_default.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    this->setStyleSheet(styleSheet);

    // 设置长按自动重复动作
    ui->pushButton->setAutoRepeat(true);

    connect(ui->pushButton, QPushButton::clicked, this, [this]() {

        // QListWidgetItem *item = new QListWidgetItem;
        // item->setText("Apple");
        // item->setIcon(QIcon(":/image/emoji_celebrate.png"));
        // ui->listWidget->addItem(item);
    });

    connect(ui->pushButton, QPushButton::clicked, this, [this]() {

        /**
         * 实现如果滚动条位置在最下面, 则总是往下滚动, 否则不要滚动
         */

        // static int i;
        // ui->listWidget->addItem("Item " + QString::number(i++));
        // QScrollBar *vbar = ui->listWidget->verticalScrollBar();
        // if(vbar->value() == vbar->maximum())
        //     ui->listWidget->scrollToBottom();

        QListWidgetItem *item1 = new QListWidgetItem;
        ListItem *listitem = new ListItem;
        // item1->setSizeHint(QSize(0, 720));
        item1->setSizeHint(QSize(0, listitem->height()));
        ui->listWidget->addItem(item1);
        ui->listWidget->setItemWidget(item1, listitem);

    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

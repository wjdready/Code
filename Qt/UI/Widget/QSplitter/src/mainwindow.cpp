#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QDebug>
#include <QTextEdit>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    qDebug() << "MainWindow start";

    ui->setupUi(this);
    
    QFile file(":/style/style_default.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    this->setStyleSheet(styleSheet);

    // QTextEdit *editor1 = new QTextEdit;
    // QTextEdit *editor2 = new QTextEdit;
    // QTextEdit *editor3 = new QTextEdit;

    
    QSplitter* splitter = new QSplitter(Qt::Vertical);
    
    splitter->addWidget(ui->textEdit);
    splitter->addWidget(ui->textEdit_2);

    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);

    ui->verticalLayout_2->removeWidget(ui->textEdit);
    ui->verticalLayout_2->removeWidget(ui->textEdit_2);
    ui->verticalLayout_2->removeItem(ui->horizontalLayout);

    ui->verticalLayout_2->addWidget(splitter);
    ui->verticalLayout_2->addLayout(ui->horizontalLayout);
    
    // splitter->setParent(ui->verticalLayout_2);

    // setCentralWidget(splitter);

}

MainWindow::~MainWindow()
{
    delete ui;
}



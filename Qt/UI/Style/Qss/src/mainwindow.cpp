#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QDebug>
#include <QIcon>
#include <QStyle>
#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    qDebug() << "MainWindow start";

    ui->setupUi(this);
    
    QFile file(":/style/style_default.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    this->setStyleSheet(styleSheet);

    // QStyle* style = QApplication::style();
    // QIcon icon = style->standardIcon(QStyle::SP_DialogCloseButton);
    // ui->pushButton->setIcon(icon);

    QFontDatabase::addApplicationFont(":/font/myfont.otf");
    ui->pushButton_2->setFont(QFont("Font Awesome 6 Free Solid", 32));
    ui->pushButton_2->setText("\uf191 Love");

    ui->label->setText(QString("<font style = 'font-family:iconfont; font-size:35px;'>%1 </font><font style = 'font-family:Microsoft YaHei; font-size:35px;'>%2</font>").arg("\ue60c", ui->label->text()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

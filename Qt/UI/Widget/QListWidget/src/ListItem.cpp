#include "ListItem.h"
#include <QFile>

ListItem::ListItem(QWidget* parent) : QWidget(parent), ui(new Ui::ListItem)
{
    ui->setupUi(this);

    QFile file(":/style/style_default.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    this->setStyleSheet(styleSheet);

    ui->label->setText("这里面涉及了不同方面的专业，策划、架构、代码编写等等，你一个人，只能完成或者与别人合作完成其中的一个方面而已。不过最基本的，想写wps一样的软件，C++是必须要学习的。如果有一天你的技术到了，真可以去应聘wps开发人员");

    ui->label->setWordWrap(true);

}   

ListItem::~ListItem()
{
    delete ui;
}

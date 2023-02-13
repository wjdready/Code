#pragma once

#include <QtWidgets/QWidget>
#include <QListWidgetItem>
#include <QListWidget>
#include <QMessageBox>
#include "ui_ListItem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ListItem; }
QT_END_NAMESPACE

class ListItem : public QWidget
{
    Q_OBJECT

public:
   ListItem(QWidget *parent = nullptr);
    ~ListItem();

private:
    Ui::ListItem *ui;
};

/********************************************************************************
** Form generated from reading UI file 'MainWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWidget
{
public:
    QHBoxLayout *horizontalLayout_16;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_9;
    QLabel *labelPortName;
    QComboBox *boxPortName;
    QHBoxLayout *horizontalLayout_10;
    QLabel *labelBaudRate;
    QComboBox *boxBaudRate;
    QHBoxLayout *horizontalLayout_11;
    QLabel *labelDataBits;
    QComboBox *boxDataBits;
    QHBoxLayout *horizontalLayout_12;
    QLabel *labelParity;
    QComboBox *boxParity;
    QHBoxLayout *horizontalLayout_13;
    QLabel *labelStopBits;
    QComboBox *boxStopBits;
    QHBoxLayout *horizontalLayout_14;
    QLabel *labelFlowControl;
    QComboBox *boxFlowControl;
    QHBoxLayout *horizontalLayout_15;
    QPushButton *btnRefresh;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btnOpen;
    QPushButton *btnConnectTest;
    QPushButton *btnGetAuthInfoTest;
    QPushButton *btnGetCalInfoTest;
    QPushButton *btnGeFileList;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout;
    QLabel *labelRecv;
    QTextEdit *textRecv;
    QLabel *labelSend;
    QTextEdit *textSend;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnSend;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *MainWidget)
    {
        if (MainWidget->objectName().isEmpty())
            MainWidget->setObjectName(QString::fromUtf8("MainWidget"));
        MainWidget->resize(548, 346);
        horizontalLayout_16 = new QHBoxLayout(MainWidget);
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        labelPortName = new QLabel(MainWidget);
        labelPortName->setObjectName(QString::fromUtf8("labelPortName"));
        labelPortName->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(labelPortName);

        boxPortName = new QComboBox(MainWidget);
        boxPortName->setObjectName(QString::fromUtf8("boxPortName"));

        horizontalLayout_9->addWidget(boxPortName);


        verticalLayout_2->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        labelBaudRate = new QLabel(MainWidget);
        labelBaudRate->setObjectName(QString::fromUtf8("labelBaudRate"));
        labelBaudRate->setAlignment(Qt::AlignCenter);

        horizontalLayout_10->addWidget(labelBaudRate);

        boxBaudRate = new QComboBox(MainWidget);
        boxBaudRate->setObjectName(QString::fromUtf8("boxBaudRate"));

        horizontalLayout_10->addWidget(boxBaudRate);


        verticalLayout_2->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        labelDataBits = new QLabel(MainWidget);
        labelDataBits->setObjectName(QString::fromUtf8("labelDataBits"));
        labelDataBits->setAlignment(Qt::AlignCenter);

        horizontalLayout_11->addWidget(labelDataBits);

        boxDataBits = new QComboBox(MainWidget);
        boxDataBits->setObjectName(QString::fromUtf8("boxDataBits"));

        horizontalLayout_11->addWidget(boxDataBits);


        verticalLayout_2->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        labelParity = new QLabel(MainWidget);
        labelParity->setObjectName(QString::fromUtf8("labelParity"));
        labelParity->setAlignment(Qt::AlignCenter);

        horizontalLayout_12->addWidget(labelParity);

        boxParity = new QComboBox(MainWidget);
        boxParity->setObjectName(QString::fromUtf8("boxParity"));

        horizontalLayout_12->addWidget(boxParity);


        verticalLayout_2->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        labelStopBits = new QLabel(MainWidget);
        labelStopBits->setObjectName(QString::fromUtf8("labelStopBits"));
        labelStopBits->setAlignment(Qt::AlignCenter);

        horizontalLayout_13->addWidget(labelStopBits);

        boxStopBits = new QComboBox(MainWidget);
        boxStopBits->setObjectName(QString::fromUtf8("boxStopBits"));

        horizontalLayout_13->addWidget(boxStopBits);


        verticalLayout_2->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        labelFlowControl = new QLabel(MainWidget);
        labelFlowControl->setObjectName(QString::fromUtf8("labelFlowControl"));
        labelFlowControl->setAlignment(Qt::AlignCenter);

        horizontalLayout_14->addWidget(labelFlowControl);

        boxFlowControl = new QComboBox(MainWidget);
        boxFlowControl->setObjectName(QString::fromUtf8("boxFlowControl"));

        horizontalLayout_14->addWidget(boxFlowControl);


        verticalLayout_2->addLayout(horizontalLayout_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        btnRefresh = new QPushButton(MainWidget);
        btnRefresh->setObjectName(QString::fromUtf8("btnRefresh"));

        horizontalLayout_15->addWidget(btnRefresh);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_3);

        btnOpen = new QPushButton(MainWidget);
        btnOpen->setObjectName(QString::fromUtf8("btnOpen"));

        horizontalLayout_15->addWidget(btnOpen);


        verticalLayout_2->addLayout(horizontalLayout_15);

        btnConnectTest = new QPushButton(MainWidget);
        btnConnectTest->setObjectName(QString::fromUtf8("btnConnectTest"));

        verticalLayout_2->addWidget(btnConnectTest);

        btnGetAuthInfoTest = new QPushButton(MainWidget);
        btnGetAuthInfoTest->setObjectName(QString::fromUtf8("btnGetAuthInfoTest"));

        verticalLayout_2->addWidget(btnGetAuthInfoTest);

        btnGetCalInfoTest = new QPushButton(MainWidget);
        btnGetCalInfoTest->setObjectName(QString::fromUtf8("btnGetCalInfoTest"));

        verticalLayout_2->addWidget(btnGetCalInfoTest);

        btnGeFileList = new QPushButton(MainWidget);
        btnGeFileList->setObjectName(QString::fromUtf8("btnGeFileList"));

        verticalLayout_2->addWidget(btnGeFileList);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_16->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelRecv = new QLabel(MainWidget);
        labelRecv->setObjectName(QString::fromUtf8("labelRecv"));

        verticalLayout->addWidget(labelRecv);

        textRecv = new QTextEdit(MainWidget);
        textRecv->setObjectName(QString::fromUtf8("textRecv"));

        verticalLayout->addWidget(textRecv);

        labelSend = new QLabel(MainWidget);
        labelSend->setObjectName(QString::fromUtf8("labelSend"));

        verticalLayout->addWidget(labelSend);

        textSend = new QTextEdit(MainWidget);
        textSend->setObjectName(QString::fromUtf8("textSend"));

        verticalLayout->addWidget(textSend);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnSend = new QPushButton(MainWidget);
        btnSend->setObjectName(QString::fromUtf8("btnSend"));

        horizontalLayout->addWidget(btnSend);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_16->addLayout(verticalLayout);

        horizontalLayout_16->setStretch(1, 1);

        retranslateUi(MainWidget);

        QMetaObject::connectSlotsByName(MainWidget);
    } // setupUi

    void retranslateUi(QWidget *MainWidget)
    {
        MainWidget->setWindowTitle(QCoreApplication::translate("MainWidget", "MainWidget", nullptr));
        labelPortName->setText(QCoreApplication::translate("MainWidget", "\344\270\262\345\217\243\345\220\215\357\274\232", nullptr));
        labelBaudRate->setText(QCoreApplication::translate("MainWidget", "\346\263\242\347\211\271\347\216\207\357\274\232", nullptr));
        labelDataBits->setText(QCoreApplication::translate("MainWidget", "\346\225\260\346\215\256\344\275\215\357\274\232", nullptr));
        labelParity->setText(QCoreApplication::translate("MainWidget", "\346\240\241\351\252\214\344\275\215\357\274\232", nullptr));
        labelStopBits->setText(QCoreApplication::translate("MainWidget", "\345\201\234\346\255\242\344\275\215\357\274\232", nullptr));
        labelFlowControl->setText(QCoreApplication::translate("MainWidget", "\346\265\201\346\216\247\345\210\266\357\274\232", nullptr));
        btnRefresh->setText(QCoreApplication::translate("MainWidget", "\345\210\267\346\226\260", nullptr));
        btnOpen->setText(QCoreApplication::translate("MainWidget", "\346\211\223\345\274\200", nullptr));
        btnConnectTest->setText(QCoreApplication::translate("MainWidget", "\350\277\236\346\216\245\346\265\213\350\257\225", nullptr));
        btnGetAuthInfoTest->setText(QCoreApplication::translate("MainWidget", "\350\216\267\345\217\226\346\216\210\346\235\203\344\277\241\346\201\257\346\265\213\350\257\225", nullptr));
        btnGetCalInfoTest->setText(QCoreApplication::translate("MainWidget", "\350\216\267\345\217\226\346\240\241\345\207\206\344\277\241\346\201\257", nullptr));
        btnGeFileList->setText(QCoreApplication::translate("MainWidget", "\350\216\267\345\217\226\346\226\207\344\273\266\345\210\227\350\241\250", nullptr));
        labelRecv->setText(QCoreApplication::translate("MainWidget", "\346\216\245\346\224\266\345\214\272\357\274\232", nullptr));
        labelSend->setText(QCoreApplication::translate("MainWidget", "\345\217\221\351\200\201\345\214\272\357\274\232", nullptr));
        btnSend->setText(QCoreApplication::translate("MainWidget", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWidget: public Ui_MainWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWIDGET_H

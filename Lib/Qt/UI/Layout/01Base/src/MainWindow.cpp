#include "ui_MainWidget.h"
#include "MainWidget.h"
#include "AWAJson.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QListView>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include "mainwindow.h"

static void boxset(QWidget *par, QComboBox *comboBox, bool editable, const QStringList &list, const char *initstr);
static void uiStyleInit(MainWidget *self, Ui::MainWidget *ui);

MainWidget::MainWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MainWidget)
{
    /* 初始化UI */
    ui->setupUi(this);
    refreshSerial();

    ui->boxPortName->setView(new QListView(this));
    boxset(this, ui->boxBaudRate, true, {"1200", "2400", "9600", "19200", "38400", "57600", "115200"}, "115200");
    boxset(this, ui->boxDataBits, false, {"5", "6", "7", "8"}, "8");
    boxset(this, ui->boxParity, false, {"无", "Even偶", "Odd奇", "Space", "Mark"}, "无");
    boxset(this, ui->boxStopBits, false, {"1", "1.5", "2"}, "1");
    boxset(this, ui->boxFlowControl, false, {"无", "Hardware", "Software"}, "无");

    /* 设置样式 */
    uiStyleInit(this, this->ui);

    connect(ui->btnOpen, &QPushButton::clicked, this, [this]() {
        if (ui->btnOpen->text() == "打开")
            openSerial();
        else
            closeSerial();
    });

    // 刷新按钮点击事件
    connect(ui->btnRefresh, QPushButton::clicked, this, refreshSerial);

    // 发送按钮点击事件
    connect(ui->btnSend, QPushButton::clicked, this, sendData);

    // 测试按钮
    connect(ui->btnConnectTest, QPushButton::clicked, this, connectTest);
    connect(ui->btnGetAuthInfoTest, QPushButton::clicked, this, getAuthInfoTest);
    connect(ui->btnGetCalInfoTest, QPushButton::clicked, this, getCaliInfoTest);
    connect(ui->btnGeFileList, QPushButton::clicked, this, getFileListTest);

    AWAJson = new AWAJsonSerial();
    QThread *thread = new QThread();
    AWAJson->moveToThread(thread);

    thread->start();

    connect(this, AWAJsonHandle, AWAJson, AWAJson::handle);
    connect(AWAJson, AWAJson::handleFinshed, this, AWAJsonHandleResult);

    serial = &(AWAJson->serial);

    connect(this, AWAJsonSendRawData, AWAJson, AWAJson::sendRawData);
}

MainWidget::~MainWidget()
{
    delete ui;
}

static void boxset(QWidget *par, QComboBox *comboBox, bool editable, const QStringList &list, const char *initstr)
{
    comboBox->addItems(list);
    comboBox->setEditable(editable);
    comboBox->setCurrentText(initstr);
    comboBox->setView(new QListView(par));
}

static void setSerialEnable(Ui::MainWidget *ui, bool enabled)
{
    ui->btnRefresh->setEnabled(enabled);
    ui->btnOpen->setText(enabled ? QString("打开") : QString("关闭"));
    ui->boxPortName->setEnabled(enabled);
    ui->boxBaudRate->setEnabled(enabled);
    ui->boxDataBits->setEnabled(enabled);
    ui->boxParity->setEnabled(enabled);
    ui->boxStopBits->setEnabled(enabled);
    ui->boxFlowControl->setEnabled(enabled);
}

static void widgetAddshadowEffect(QWidget *widget)
{
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(5);
    effect->setXOffset(0);
    effect->setYOffset(0);
    effect->setColor(QColor(0, 0, 0, 100));
    widget->setGraphicsEffect(effect);
}

static void uiStyleInit(MainWidget *self, Ui::MainWidget *ui)
{
    QFile file(":/style/style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    self->setStyleSheet(styleSheet);

    widgetAddshadowEffect(ui->textSend);
    widgetAddshadowEffect(ui->textRecv);
    widgetAddshadowEffect(ui->boxBaudRate);
    widgetAddshadowEffect(ui->boxDataBits);
    widgetAddshadowEffect(ui->boxParity);
    widgetAddshadowEffect(ui->boxFlowControl);
    widgetAddshadowEffect(ui->boxStopBits);
    widgetAddshadowEffect(ui->boxPortName);

    widgetAddshadowEffect(ui->btnOpen);
    widgetAddshadowEffect(ui->btnRefresh);
    widgetAddshadowEffect(ui->btnSend);
    widgetAddshadowEffect(ui->btnConnectTest);
}

/* 打开串口 */
void MainWidget::openSerial()
{
    const QString portName = ui->boxPortName->currentText();
    if (portName.isEmpty())
    {
        qDebug() << "未找到可用串口,请确认串口连接正常后点击刷新";
        return;
    }

    if (QSerialPortInfo(portName).isBusy())
    {
        qDebug() << "串口 " << portName << " 繁忙, 可能已被占用,请确认后再连接";
        return;
    }

    qint32 baudRate = ui->boxBaudRate->currentText().toInt();
    serial->setPortName(portName);
    serial->setBaudRate(baudRate);
    if (serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "串口已打开,读写模式";
        setSerialEnable(this->ui, false);

        return;
    }

    qDebug() << "串口打开异常" << portName << serial->errorString();
    serial->clearError();
    setSerialEnable(this->ui, true);
}

/* 关闭串口 */
void MainWidget::closeSerial()
{
    serial->clear();
    serial->close();
    setSerialEnable(this->ui, true);
}

/* 刷新串口 */
void MainWidget::refreshSerial()
{
    ui->boxPortName->clear();

    QStringList slist;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if (!info.isBusy())
            slist << info.portName();
    }

    if (slist.isEmpty())
    {
        qDebug() << "未找到可用串口,请确认串口连接正常后点击刷新";
        return;
    }

    ui->boxPortName->addItems(slist);
}

/* 发送原始数据 */
void MainWidget::sendData()
{
    const QByteArray data = ui->textSend->toPlainText().toUtf8();
    ui->textRecv->append("发送: " + QString::fromUtf8(data));

    emit AWAJsonSendRawData(data, 1000);
}

/* 连接测试 */
void MainWidget::connectTest()
{
    emit AWAJsonHandle(AWAJson::CLASS_101, 2000);
}

void MainWidget::getAuthInfoTest()
{
    emit AWAJsonHandle(AWAJson::CLASS_130, 2000);
}

void MainWidget::getCaliInfoTest()
{
    emit AWAJsonHandle(AWAJson::CLASS_131, 2000);
}

void MainWidget::getFileListTest()
{
    emit AWAJsonHandle(AWAJson::CLASS_135, 2000);
}

/* 将处理结果放到 UI 展示 */
void MainWidget::AWAJsonHandleResult(int type, QByteArray *data)
{
    if (data == NULL)
    {
        ui->textRecv->append("读取或写入失败");
        return ;
    }

    switch (type)
    {
    case AWAJson::CLASS_101:

        break;

    default:
        break;
    }

    ui->textRecv->append("接收: " + QString::fromUtf8(*data));

    delete data;
}

/*
    链接测试: <N>{"Class":"101","Flag":"20230207125348000"}</N>
    获取授权信息: <N>{"Class":"130","Flag":"20230209100502000"}</N>
    获取校准信息数量: <N>{"Class":"131","Flag":"20230209100525000","CMD":0}</N>
    获取目录信息: <N>{"Class":"135","Flag":"20230209100558000","DeviceAWAID":"60021","CMD":0,"Path":"/"}</N>
    接收文件测试:
   <N>{"Class":"135","Flag":"20230207111324000","DeviceAWAID":"60021","CMD":1,"Path":"/data/2000-01-01/042100/audio_00.wav"}</N>
*/

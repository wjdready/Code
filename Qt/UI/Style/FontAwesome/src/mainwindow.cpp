#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QDebug>
#include <QStandardItem>

void MainWindow::styleChanged(int index)
{
    int style = index;

    QHash<QString, int> iconset = awesome->namedCodePoints(style);

    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>(ui->listView->model());
    model->clear();

    for (QHash<QString, int>::iterator i=iconset.begin(); i != iconset.end(); ++i) {
        QString name = i.key();
        int ic = i.value();
        model->appendRow(new QStandardItem(awesome->icon(style, ic), name));
    }
    model->sort(0);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    qDebug() << "MainWindow start";

    ui->setupUi(this);
    
    QFile file(":/style/style_default.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    this->setStyleSheet(styleSheet);

    // fa::QtAwesome* awesome = new fa::QtAwesome(qApp)
    // awesome->initFontAwesome();

    awesome = new fa::QtAwesome(qApp);
    awesome->initFontAwesome();  
    
    // ui->pushButton->setIcon(awesome->icon(fa::fa_solid, fa::fa_caret_square_right));
    QVariantMap options;
    options.insert("anim", QVariant::fromValue(new fa::QtAwesomeAnimation(ui->pushButton)));
    ui->pushButton->setIcon(awesome->icon(fa::fa_solid, fa::fa_beer_mug_empty, options));

    QStandardItemModel* model = new QStandardItemModel(this);
    ui->listView->setModel(model);

    connect(ui->pushButton, QPushButton::clicked, this, [this]() {
        static int flag;
        flag = (flag + 1) % 4;
        
        if(flag == 0)
            ui->pushButton->setIcon(awesome->icon(fa::fa_solid, fa::fa_caret_square_down));
        else if(flag == 1)
            ui->pushButton->setIcon(awesome->icon(fa::fa_solid, fa::fa_caret_square_left));
        else if(flag == 2)
            ui->pushButton->setIcon(awesome->icon(fa::fa_solid, fa::fa_caret_square_up));
        else if(flag == 3)
            ui->pushButton->setIcon(awesome->icon(fa::fa_solid, fa::fa_caret_square_right));

        // awesome->give("duplicate", new DuplicateIconPainter());

        ui->label->setFont(awesome->font(fa::fa_solid, 32));
        ui->label->setText(QString("%1 非常好").arg(QChar(fa::fa_caret_square_right)));
        // ui->label_2->setText(QString("非常好 %1 >").arg(QChar(0xf815)));

        // ui->label->setPixmap(awesome->icon(fa::fa_solid, fa::fa_credit_card).pixmap(32, 32));
        // ui->label->setText("My Credit Card");

        styleChanged(0);

        qDebug() << "Clicked";
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

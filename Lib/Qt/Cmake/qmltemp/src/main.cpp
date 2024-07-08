#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // 设置全局字体
    int fontId = QFontDatabase::addApplicationFont(":/font/SourceHanSansCN-Medium.otf");
    if (fontId != -1)
    {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.isEmpty())
        {
            QString familyName = fontFamilies.first();
            QFont font(familyName);
            app.setFont(font);
        }
    }

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}

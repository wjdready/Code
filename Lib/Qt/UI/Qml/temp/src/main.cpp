#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#if USE_FELGO_LIVE
#include <FelgoLiveClient>
#include <FelgoApplication>
#endif

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

#if USE_FELGO_LIVE
    FelgoApplication felgo;
    felgo.initialize(&engine);
    // felgo.setMainQmlFileName(QStringLiteral("qrc:/qml/main.qml"));
    // engine.load(QUrl(felgo.mainQmlFileName()));
    
    FelgoLiveClient liveClient(&engine);
#else

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

#endif

    return app.exec();
}

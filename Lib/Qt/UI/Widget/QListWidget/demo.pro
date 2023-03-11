QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp  \
    src/ListItem.cpp

HEADERS += \
    src/mainwindow.h    \
    src/ListItem.h

CONFIG += debug console

INCLUDEPATH  += resource/ui

FORMS += \
    resource/ui/mainwindow.ui   \
    resource/ui/ListItem.ui

RESOURCES += $$_PRO_FILE_PWD_/resource/resource.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

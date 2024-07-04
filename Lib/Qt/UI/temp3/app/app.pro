
QT += core gui widgets

TEMPLATE = app

SOURCES += \
    src/main.cpp    \
    src/mainwindow.cpp

HEADERS += \
    src/mainwindow.h

INCLUDEPATH  += \
    resource/ui

CONFIG += console

RESOURCES += $$_PRO_FILE_PWD_/resource/resource.qrc

FORMS += \
    resource/ui/mainwindow.ui

TARGET = app


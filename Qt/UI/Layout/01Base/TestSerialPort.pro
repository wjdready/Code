#-------------------------------------------------
#
# Project created by QtCreator 2019-02-16T10:11:58
#
#-------------------------------------------------

QT       += core gui widgets serialport

TARGET = TestSerialPort
TEMPLATE = app

CONFIG += console
CONFIG += debug


SOURCES +=                          \
    src/main.cpp                    \
    src/MainWidget.cpp              \
    src/AWAJson.cpp                 \
    src/AWAJsonSerial.cpp


HEADERS  +=                         \
    src/MainWidget.h                \
    src/AWAJson.h                   \
    src/AWAJsonSerial.h

INCLUDEPATH  += resource/ui


FORMS +=                            \
    resource/ui/MainWidget.ui


RESOURCES += $$_PRO_FILE_PWD_/resource/resource.qrc




TEMPLATE = app

SOURCES += src/main.cpp

CONFIG += console

LIBS += -L../bin -lfun1 -lfun2

INCLUDEPATH += ../libfun1_dll ../libfun2_static

TARGET = demo

DESTDIR = ../bin

# CONFIG += static

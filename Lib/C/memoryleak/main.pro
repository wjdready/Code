QT += core gui widgets

SOURCES += main.cpp

CONFIG += c++11 debug console

QMAKE_CXXFLAGS += -g -O0 -fsanitize=address
QMAKE_LFLAGS += -fsanitize=address

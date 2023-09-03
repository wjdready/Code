QT += core gui widgets
TARGET = qtdemo

SOURCES += main.cpp

QMAKE_CFLAGS += --sysroot ../_rootfs
QMAKE_CXXFLAGS += --sysroot ../_rootfs
QMAKE_LFLAGS += --sysroot ../_rootfs

QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]

message(QT_SYSROOT = $$[QT_SYSROOT])

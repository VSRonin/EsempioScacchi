QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = Scacchi
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11
SOURCES += \
        main.cpp \
    scacchiera.cpp \
    pezzodelegate.cpp \
    pezzo.cpp

HEADERS += \
    pezzo.h \
    scacchiera.h \
    pezzodelegate.h \
    scacchiglobal.h

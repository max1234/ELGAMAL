#-------------------------------------------------
#
# Project created by QtCreator 2013-11-15T22:50:45
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = elgamal
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    elgamal.cpp \
    algorithms.cpp



HEADERS += \
    elgamal.h \
    algorithms.h

unix|win32: LIBS += -L$$PWD/ -llibgmp-3

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

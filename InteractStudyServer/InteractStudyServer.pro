#-------------------------------------------------
#
# Project created by QtCreator 2016-11-12T12:18:12
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = InteractStudyServer
TEMPLATE = app


SOURCES += main.cpp\
        interactserver.cpp

HEADERS  += interactserver.h \
    person.h

RESOURCES += \
    img.qrc

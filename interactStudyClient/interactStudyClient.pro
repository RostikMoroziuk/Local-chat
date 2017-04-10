#-------------------------------------------------
#
# Project created by QtCreator 2016-11-12T15:15:08
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = interactStudyClient
TEMPLATE = app


SOURCES += main.cpp\
        start.cpp \
    teacher.cpp \
    student.cpp \
    addstudent.cpp

HEADERS  += start.h \
    teacher.h \
    student.h \
    person.h \
    addstudent.h

FORMS    += start.ui \
    teacher.ui \
    student.ui \
    addstudent.ui

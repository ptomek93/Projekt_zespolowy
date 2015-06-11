#-------------------------------------------------
#
# Project created by QtCreator 2015-06-11T18:36:47
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Master
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    modbus.cpp \
    settingsdialog.cpp \
    form.cpp \
    zaluzje.cpp \
    alarm.cpp \
    swiatlo.cpp

HEADERS  += mainwindow.h \
    modbus.h \
    settingsdialog.h \
    form.h \
    zaluzje.h \
    alarm.h \
    swiatlo.h \
    struktura.hh

FORMS    += mainwindow.ui \
    modbus.ui \
    settingsdialog.ui \
    form.ui \
    zaluzje.ui \
    alarm.ui \
    swiatlo.ui

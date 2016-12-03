#-------------------------------------------------
#
# Project created by QtCreator 2016-10-07T17:32:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmsKeeper
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    message.cpp \
    addmessagedialog.cpp \
    database.cpp \
    addcompaniondialog.cpp

HEADERS  += mainwindow.h \
    message.h \
    addmessagedialog.h \
    database.h \
    addcompaniondialog.h

FORMS    += mainwindow.ui \
    addmessagedialog.ui \
    addcompaniondialog.ui

RESOURCES += \
    resources.qrc

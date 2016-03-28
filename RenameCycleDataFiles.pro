#-------------------------------------------------
#
# Project created by QtCreator 2015-12-06T10:09:39
#
#-------------------------------------------------

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = RenameCycleDataFiles
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fileHandleClass.cpp \
    clsdirwatcher.cpp

HEADERS  += mainwindow.h \
    fileHandleClass.h \
    clsdirwatcher.h

FORMS    += mainwindow.ui

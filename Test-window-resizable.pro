#-------------------------------------------------
#
# Project created by QtCreator 2013-01-26T17:11:32
#
#-------------------------------------------------

include(3rd-party/qextserialport-1.2beta2/src/qextserialport.pri)
CONFIG += static

QT       += core gui

TARGET = Test-window-resizable
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    subwindowplot.cpp \
    3rd-party/qcustomplot.cpp \
    plotmdiwindow.cpp \
    plotdialog.cpp \
    refcontrolpanel.cpp \
    mspcomdialog.cpp \
    changevariablesdialog.cpp \
    interruptorwidget.cpp \
    integerwidget.cpp \
    doublewidget.cpp \
    refdialog.cpp \
    refdialogitem.cpp

HEADERS  += mainwindow.h \
    subwindowplot.h \
    3rd-party/qcustomplot.h \
    plotmdiwindow.h \
    plotdialog.h \
    refcontrolpanel.h \
    mspcomdialog.h \
    referencevariable.h \
    abstractvariable.h \
    changevariablesdialog.h \
    interruptorwidget.h \
    integerwidget.h \
    doublewidget.h \
    refdialog.h \
    refdialogitem.h

FORMS    += mainwindow.ui \
    subwindowplot.ui \
    plotdialog.ui \
    refcontrolpanel.ui \
    mspcomdialog.ui \
    changevariablesdialog.ui \
    interruptorwidget.ui \
    integerwidget.ui \
    doublewidget.ui \
    refdialog.ui \
    refdialogitem.ui

RESOURCES += \
    test.qrc

#-------------------------------------------------
#
# Project created by QtCreator 2013-02-28T20:10:39
#
#-------------------------------------------------

include(3rd-party/qextserialport-1.2beta2/src/qextserialport.pri)

#QTPLUGIN += qgif qjpeg

CONFIG += static

#CONFIG(debug, debug|release) {
#		QMAKE_LFLAGS += /NODEFAULTLIB:msvcprtd
#	}

QT       += core gui

TARGET = Memoria-2013
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
#    subwindowplot.cpp \
    3rd-party/QCustomPlot/qcustomplot.cpp \
    plotmdiwindow.cpp \
    plotdialog.cpp \
    refcontrolpanel.cpp \
    mspcomdialog.cpp \
    changevariablesdialog.cpp \
    interruptorwidget.cpp \
    integerwidget.cpp \
    doublewidget.cpp \
    refdialog.cpp \
    refdialogitem.cpp \
    dynamicqobject.cpp \
    subwindowplot2.cpp \
#    referencemdiwindow.cpp \
    referencemditemplate.cpp \
    comunicationcontroller.cpp \
    comdefinitions.cpp \
    abstractvariablehandler.cpp \
    wizarddialog.cpp \
    genericvariableitem.cpp \
    communicationdatacontainer.cpp \
    cucdutil.cpp \
    variablerangesdialog.cpp \
    consolecommandwidget.cpp \
    commandelement.cpp \
    csvexporter.cpp \
    exportcsvdialog.cpp

HEADERS  += mainwindow.h\
#    subwindowplot.h \
    3rd-party/QCustomPlot/qcustomplot.h \
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
    refdialogitem.h \
    dynamicqobject.h \
    subwindowplot2.h \
#   referencemdiwindow.h \
    referencemditemplate.h \
    comunicationcontroller.h \
    comdefinitions.h \
    abstractvariablehandler.h \
    wizarddialog.h \
    genericvariableitem.h \
    communicationdatacontainer.h \
    cucdutil.h \
    PCToDSPVariableWidget.h \
    variablerangesdialog.h \
    consolecommandwidget.h \
    commandelement.h \
    csvexporter.h \
    exportcsvdialog.h

FORMS    += mainwindow.ui\
    subwindowplot.ui \
    plotdialog.ui \
    refcontrolpanel.ui \
    mspcomdialog.ui \
    changevariablesdialog.ui \
    interruptorwidget.ui \
    integerwidget.ui \
    doublewidget.ui \
    refdialog.ui \
    refdialogitem.ui \
    subwindowplot2.ui \
    wizarddialog.ui \
    genericvariableitem.ui \
    variablerangesdialog.ui \
    consolecommandwidget.ui \
    commandelement.ui \
    exportcsvdialog.ui

RESOURCES += test.qrc \

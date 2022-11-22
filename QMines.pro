#-------------------------------------------------
#
# Project created by QtCreator 2012-11-02T22:31:20
#
#-------------------------------------------------

QT       += core gui widgets multimedia

TARGET = QMines
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mainframe.cpp \
    finishdialog.cpp \
    resultdialog.cpp \
    colorsettingsdialog.cpp \
    clickablelabel.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    mainframe.h \
    finishdialog.h \
    resultdialog.h \
    colorsettingsdialog.h \
    clickablelabel.h \
    settings.h

FORMS    += mainwindow.ui \
    finishdialog.ui \
    resultdialog.ui \
    colorsettingsdialog.ui

RESOURCES += \
    main.qrc

RC_FILE = main.rc

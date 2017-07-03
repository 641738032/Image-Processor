#-------------------------------------------------
#
# Project created by QtCreator 2017-04-06T15:51:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpatialFilter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mypixmapitem.cpp \
    imageprocess.cpp \
    dialog.cpp \
    lineardialog.cpp \
    gaussblurdialog.cpp \
    bilateralfilterdialog.cpp \
    medianfilterdialog.cpp \
    maximumfilterdialog.cpp \
    minimumdialog.cpp \
    fft.cpp \
    transformdialog.cpp \
    lowpassdialog.cpp \
    highpassdialog.cpp

HEADERS  += mainwindow.h \
    mypixmapitem.h \
    imageprocess.h \
    dialog.h \
    lineardialog.h \
    gaussblurdialog.h \
    bilateralfilterdialog.h \
    medianfilterdialog.h \
    maximumfilterdialog.h \
    minimumdialog.h \
    fft.h \
    matrix.h \
    transformdialog.h \
    lowpassdialog.h \
    highpassdialog.h

FORMS    += mainwindow.ui \
    dialog.ui \
    lineardialog.ui \
    gaussblurdialog.ui \
    bilateralfilterdialog.ui \
    medianfilterdialog.ui \
    maximumfilterdialog.ui \
    minimumdialog.ui \
    transformdialog.ui \
    lowpassdialog.ui \
    highpassdialog.ui

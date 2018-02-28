#-------------------------------------------------
#
# Project created by QtCreator 2018-02-16T11:11:19
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DentalBiometry
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# OpenCV configuration
QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig
PKGCONFIG += opencv

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    segmentation.cpp \
    tracing.cpp \
    histogram.cpp \
    filtering.cpp \
    controller.cpp \
    helpers.cpp

HEADERS += \
        mainwindow.h \
    segmentation.h \
    tracing.h \
    histogram.h \
    filtering.h \
    controller.h \
    helpers.h \
    spline.h

FORMS += \
        mainwindow.ui

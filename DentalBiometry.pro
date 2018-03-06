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
    Controller/controller.cpp \
    Model/cqtopencvviewergl.cpp \
    Model/filters.cpp \
    Model/helpers.cpp \
    Model/histogram.cpp \
    Model/segmentation.cpp \
    Model/tracing.cpp \
    Model/visualizationhelpers.cpp \
    View/mainwindow.cpp \
    main.cpp

HEADERS += \
    Controller/controller.h \
    Model/cqtopencvviewergl.h \
    Model/filters.h \
    Model/helpers.h \
    Model/histogram.h \
    Model/segmentation.h \
    Model/spline.h \
    Model/tracing.h \
    Model/visualizationhelpers.h \
    View/mainwindow.h

FORMS += \
    View/mainwindow.ui

SUBDIRS += \
    DentalBiometry.pro

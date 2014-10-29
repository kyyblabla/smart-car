#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T17:52:50
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = My3dTest
TEMPLATE = app

CONFIG += qt3d

SOURCES += main.cpp\
    factory/carmodel.cpp \
    ui/mainview.cpp \
    factory/arrownode.cpp \
    factory/devicenode.cpp \
    factory/basenode.cpp \
    factory/factoryview.cpp

HEADERS  += \
    factory/view_connmon.h \
    factory/carmodel.h \
    ui/mainview.h \
    factory/arrownode.h \
    factory/devicenode.h \
    factory/basenode.h \
    factory/factoryview.h

FORMS    += \
    ui/mainview.ui

RESOURCES += \
    rs/rs.qrc

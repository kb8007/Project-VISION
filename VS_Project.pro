
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VS_Project
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
        main.cpp \
        mainwindow.cpp \

HEADERS += \
        mainwindow.h \

FORMS += \
        mainwindow.ui\



#CONFIG += link_pkgconfig
PKGCONFIG += opencv

LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgcodecs \
-lopencv_highgui  \
-lopencv_imgproc \
-lopencv_core \
-lopencv_world

#-------------------------------------------------
#
# Project created by QtCreator 2015-03-17T16:06:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = locationDecoder
TEMPLATE = app
QMAKE_CFLAGS_WARN_OFF = -Wformat

LIBS += `pkg-config opencv --libs`

SOURCES += main.cpp\
        mainwindow.cpp \
    imgsegment.c \
    camera.cpp \
    list_impl.c \
    list.c \
    imgproc.c \
    neighbours.c \
    grid.c \
    decoder.c

HEADERS  += mainwindow.h \
    imgsegment.h \
    camera.h \
    list.h \
    list_impl.h \
    imgproc.h \
    neighbours.h \
    grid.h \
    decoder.h

FORMS    += mainwindow.ui

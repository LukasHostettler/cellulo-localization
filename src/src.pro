#-------------------------------------------------
#
# Project created by QtCreator 2015-04-20T13:46:50
#
#-------------------------------------------------

QT       -= core gui

TARGET = cellulo_localization
TEMPLATE = lib

DEFINES += CELLULO_LOCALIZATION_LIBRARY

SOURCES +=decoder.c \
    grid.c \
    imgproc.c \
    imgsegment.c \
    list.c \
    list_impl.c \
    neighbours.c \
    localization.c

HEADERS +=decoder.h \
    grid.h \
    imgproc.h \
    imgsegment.h \
    list.h \
    list_impl.h \
    neighbours.h \
    ../include/cellulo_localization.h \
    localization.h \
    ../include/cellulo_localization_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

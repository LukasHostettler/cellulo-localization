#-------------------------------------------------
#
# Project created by QtCreator 2015-04-20T13:46:50
#
#-------------------------------------------------

QT       -= core gui

TARGET = cellulo_localization
TEMPLATE = lib

DEFINES += CELLULO_LOCALIZATION_LIBRARY

SOURCES +=src/decoder.c \
    src/grid.c \
    src/imgproc.c \
    src/imgsegment.c \
    src/list.c \
    src/list_impl.c \
    src/neighbours.c \
    src/localization.c

HEADERS +=src/decoder.h \
    src/grid.h \
    src/imgproc.h \
    src/imgsegment.h \
    src/list.h \
    src/list_impl.h \
    src/neighbours.h \
    include/cellulo_localization.h \
    src/localization.h \
    include/cellulo_localization_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

#-------------------------------------------------
#
# Project created by QtCreator 2015-03-17T16:06:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cellulo_localization_test
TEMPLATE = app
QMAKE_CFLAGS_WARN_OFF = -Wformat

LIBS += `pkg-config opencv --libs`\


SOURCES += main.cpp\
    ../src/imgsegment.c \
    camera.cpp \
    ../src/list_impl.c \
    ../src/list.c \
    ../src/imgproc.c \
    ../src/neighbours.c \
    ../src/grid.c \
    ../src/decoder.c

HEADERS  += \
    ../include/cellulo_localization.h\
    ../src/imgsegment.h \
    camera.h \
    ../src/list.h \
    ../src/list_impl.h \
    ../src/imgproc.h \
    ../src/neighbours.h \
    ../src/grid.h \
    ../src/decoder.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../lib/release/ -lcellulo_localization
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../lib/debug/ -lcellulo_localization
else:unix: LIBS += -L$$PWD/../lib/ -lcellulo_localization

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include

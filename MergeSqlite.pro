#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T10:11:13
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MergeSqlite
TEMPLATE = app


SOURCES += $$PWD/src/main.cpp\
        $$PWD/src/widget.cpp\
        $$PWD/src/ntask.cpp

HEADERS  += $$PWD/inc/widget.h \
        $$PWD/inc/ntask.h

FORMS    += $$PWD/ui/widget.ui

INCLUDEPATH += $$PWD/inc/ \

win32{
    CONFIG += debug_and_release
    CONFIG(release, debug|release) {
            target_path = ./build_/dist
        } else {
            target_path = ./build_/debug
        }
        DESTDIR = ./bin
        MOC_DIR = $$target_path/moc
        RCC_DIR = $$target_path/rcc
        OBJECTS_DIR = $$target_path/obj
}

win32: LIBS += -L$$PWD/bin/ -lNBaseUiKit

INCLUDEPATH += $$PWD/3rd/inc
DEPENDPATH += $$PWD/3rd/inc

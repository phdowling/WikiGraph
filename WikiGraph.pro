#-------------------------------------------------
#
# Project created by QtCreator 2011-12-14T18:14:41
#
#-------------------------------------------------

QT       += core gui \
            network \
            opengl

TARGET = WikiGraph
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphwidget.cpp \
    edge.cpp \
    nodeedge.cpp \
    wikipediagraphwidget.cpp \
    node.cpp \
    wikinode.cpp \
    pageloader.cpp \
    graphlayout.cpp \
    directforcelayout.cpp \
    fastlayout.cpp \
    quadtreenode.cpp \
    barneshutlayout.cpp \
    sliderswidget.cpp

HEADERS  += mainwindow.h \
    graphwidget.h \
    edge.h \
    nodeedge.h \
    wikipediagraphwidget.h \
    node.h \
    wikinode.h \
    pageloader.h \
    graphlayout.h \
    fastlayout.h \
    fieldmap.h \
    newton.h \
    directforcelayout.h \
    quadtreenode.h \
    barneshutlayout.h \
    sliderswidget.h

FORMS    += mainwindow.ui \
    graphwidget.ui \
    sliderswidget.ui

OTHER_FILES += \
    todo.txt

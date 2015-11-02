#-------------------------------------------------
#
# Project created by QtCreator 2015-10-19T12:20:36
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtcceditor
TEMPLATE = app
DESTDIR = $$PWD/bin

CONFIG += c++11
DEFINES += RAPIDXML_NO_EXCEPTIONS COCOS2D_DEBUG=1 BUILD_EDITOR \
    QT_DLL QT_CORE_LIB QT_GUI_LIB QT_OPENGL_LIB QT_WIDGETS_LIB

QMAKE_CXXFLAGS += -std=c++11 -Wno-deprecated-declarations -Wno-reorder

include(third_party/qtpropertybrowser/src/qtpropertybrowser.pri)
include(libcocos2d.pri)
include(Classes/libeditor.pri)

RESOURCES += 

INCLUDEPATH += Classes \
    cocos2d/cocos \
    cocos2d/external \
    third_party \
    third_party/qtpropertybrowser/src

macx{
    OBJECTIVE_SOURCES += proj.mac/EditorPlatform.mm

    LIBS    += -framework Cocoa
    LIBS    += -framework Foundation
    LIBS    += -framework OpenAL
    LIBS    += -framework CoreVideo
}

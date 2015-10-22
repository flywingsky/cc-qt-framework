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
DEFINES += RAPIDXML_NO_EXCEPTIONS COCOS2D_DEBUG=1 \
    QC_EDITOR QT_DLL QT_CORE_LIB QT_GUI_LIB QT_OPENGL_LIB QT_WIDGETS_LIB

QMAKE_CXXFLAGS = -std=c++11 -Wno-deprecated-declarations -Wno-reorder

CC_EXTERNAL_DIR = $$PWD/cocos2d/external

HEADERS += \ 
    Classes/AppDelegate.h \
    Classes/glwidget.h \
    Classes/HelloWorldScene.h \
    Classes/mainwindow.h \
    Classes/FileTools.h \
    Classes/QtGLViewImpl.h \
    Classes/uiloader/loaders/CCNodeLoader.h \
    Classes/uiloader/loaders/CCSpriteLoader.h \
    Classes/uiloader/BaseLoader.h \
    Classes/uiloader/Singleton.h \
    Classes/uiloader/UIHelper.h \
    Classes/uiloader/UILoader.h \
    Classes/uiloader/UIMacros.h

SOURCES += \ 
    Classes/AppDelegate.cpp \
    Classes/glwidget.cpp \
    Classes/HelloWorldScene.cpp \
    Classes/main.cpp \
    Classes/mainwindow.cpp \
    Classes/FileTools.cpp \
    Classes/QtGLViewImpl.cpp \
    Classes/uiloader/loaders/CCNodeLoader.cpp \
    Classes/uiloader/loaders/CCSpriteLoader.cpp \
    Classes/uiloader/BaseLoader.cpp \
    Classes/uiloader/UIHelper.cpp \
    Classes/uiloader/UILoader.cpp

FORMS += \ 
    Classes/mainwindow.ui

RESOURCES += 

INCLUDEPATH += Classes \
    cocos2d/cocos \
    cocos2d/external \
    third_party

macx{
    OBJECTIVE_SOURCES += proj.mac/EditorPlatform.mm

    LIBS    += -framework Cocoa
    LIBS    += -framework Foundation
    LIBS    += -framework OpenAL
    LIBS    += -framework CoreVideo

    # prebuilt libraries
    INCLUDEPATH += $${CC_EXTERNAL_DIR}/png/include/mac
    LIBS        += $${CC_EXTERNAL_DIR}/png/prebuilt/mac/libpng.a

    INCLUDEPATH += $${CC_EXTERNAL_DIR}/tiff/include/mac
    LIBS        += $${CC_EXTERNAL_DIR}/tiff/prebuilt/mac/libtiff.a

    INCLUDEPATH += $${CC_EXTERNAL_DIR}/jpeg/include/mac
    LIBS        += $${CC_EXTERNAL_DIR}/jpeg/prebuilt/mac/libjpeg.a

    INCLUDEPATH += $${CC_EXTERNAL_DIR}/curl/include/mac
    LIBS        += $${CC_EXTERNAL_DIR}/curl/prebuilt/mac/libcurl.a

    INCLUDEPATH += $${CC_EXTERNAL_DIR}/glfw3/include/mac
    LIBS        += $${CC_EXTERNAL_DIR}/glfw3/prebuilt/mac/libglfw3.a

    INCLUDEPATH += $${CC_EXTERNAL_DIR}/chipmunk/include/mac
    LIBS        += $${CC_EXTERNAL_DIR}/chipmunk/prebuilt/mac/libchipmunk.a

    INCLUDEPATH += $${CC_EXTERNAL_DIR}/freetype2/include/mac
    LIBS        += $${CC_EXTERNAL_DIR}/freetype2/prebuilt/mac/libfreetype.a

    INCLUDEPATH += $${CC_EXTERNAL_DIR}/websockets/include/mac
    LIBS        += $${CC_EXTERNAL_DIR}/websockets/prebuilt/mac/libwebsockets.a

    INCLUDEPATH += $${CC_EXTERNAL_DIR}/webp/include/mac
    LIBS        += $${CC_EXTERNAL_DIR}/webp/prebuilt/mac/libwebp.a

}

QMAKE_LIBDIR += $$PWD/cocos2d-build/lib

LIBS += -lz -lcocos2d -lxxhash -lunzip -ltinyxml2 -lflatbuffers #-lrecast -lbullet -liconv

INCLUDEPATH += $$PWD/third_party/qtpropertybrowser/src
LIBS += -L$$PWD/third_party/qtpropertybrowser/lib -lQtSolutions_PropertyBrowser-head

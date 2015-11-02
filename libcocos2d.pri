
DEFINES += USE_FILE32API CC_ENABLE_CHIPMUNK_INTEGRATION=1 CC_TARGET_OS_MAC
CC_EXTERNAL_DIR = $$PWD/cocos2d/external

macx{

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

LIBS += -L$$PWD/cocos2d-build/lib

LIBS += -lz -lxxhash -lunzip -ltinyxml2 -lflatbuffers -lrecast -lbullet -liconv
LIBS += -lcocos2d

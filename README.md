# cc-qt-framework
cocos2d-x editor framework implemented by qt.

-----------------
##准备
* Qt5.4+
* CMake2.8+
* cocos2d-x3.4

##运行
* 将cocos2d-x3.4放在根目录，起名cocos2d
* 用Qt creator打开qtcceditor.pro
* 用Qt creator导入`cocos2d/CMakeLists.txt`，目标目录设为`cocos2d-build`

##注意
目前只支持cocos2d-x3.4，新版本cocos2d-x暂时渲染不出来，还在找原因，可能跟FrameBuffer有关。


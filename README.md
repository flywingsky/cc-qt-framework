# cc-qt-framework
cocos2d-x editor framework implemented by qt.
此项目已转移至：https://github.com/youlanhai/qco-editor

-----------------
## 准备
* Qt5.4+
* CMake2.8+
* cocos2d-x3.x

## 运行
* 将cocos2d-x3.x放在根目录，起名cocos2d
* 用Qt creator导入`cocos2d/CMakeLists.txt`，目标目录设为：`cocos2d-build`，CMake参数为：`-DBUILD_CPP_TESTS=OFF -DBUILD_LUA_LIBS=OFF  -DBUILD_JS_LIBS=OFF`，然后点击*执行CMake*
* 用Qt creator打开qtcceditor.pro

## 注意
此框架是一个试验品，不是完整产品，仅用于验证qt开发cocos编辑器的可行性。

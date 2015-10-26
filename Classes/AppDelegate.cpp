#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "QtGLViewImpl.h"
#include "FileTools.h"

#include "editor/Editor.h"
#include "editor/LogTool.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
    Editor::Editor::initInstance();
}

AppDelegate::~AppDelegate() 
{
    Editor::Editor::finiInstance();
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();

    auto glview = director->getOpenGLView();
    CCAssert(glview != nullptr, "glview shouldn't be null");
    //glview->setDesignResolutionSize(960, 640, ResolutionPolicy::SHOW_ALL);

    std::string resPath = Editor::getFilePath(Editor::getAppModulePath()) + "Resources";
    LOG_DEBUG("Resources Path: %s", resPath.c_str());
    FileUtils::getInstance()->addSearchPath(resPath);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    if(!Editor::Editor::instance()->init())
    {
        return false;
    }

    Editor::Editor::instance()->setRootNode(scene);

    Node *sprite = scene->getChildByName("layer")->getChildByName("sprite");
    CCAssert(sprite, "");
    Editor::Editor::instance()->setTargetNode(sprite);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

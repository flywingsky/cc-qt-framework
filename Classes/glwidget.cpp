
#include "glwidget.h"

#include <QTimer>
#include <base/CCDirector.h>
#include <platform/CCApplication.h>
#include <renderer/CCRenderer.h>
#include "QtGLViewImpl.h"

USING_NS_CC;

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , mouseMoveFunc(NULL)
    , mousePressFunc(NULL)
    , mouseReleaseFunc(NULL)
    , keyEventFunc(NULL)
    , timer_(NULL)
{
}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    CCLOG("GLWidget::initializeGL");

    Application::getInstance()->initGLContextAttrs();

    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview)
    {
        glview = QtGLViewImpl::create(this);
        director->setOpenGLView(glview);
    }

    Application::getInstance()->applicationDidFinishLaunching();

    timer_ = new QTimer();
    connect(timer_, SIGNAL(timeout()), this, SLOT(update()));

    timer_->start(1000 / 60);
}

void GLWidget::paintGL()
{
    Director::getInstance()->mainLoop();
}

void GLWidget::resizeGL(int width, int height)
{
    auto director = cocos2d::Director::getInstance();
    GLView* view = director->getOpenGLView();
    if (view)
    {
       view->setFrameSize(width, height);
       view->setDesignResolutionSize(width, height, ResolutionPolicy::SHOW_ALL);
       view->setViewPortInPoints(0, 0, width, height);
       director->setProjection(director->getProjection());
    }
}

void GLWidget::setMouseMoveFunc(PTRFUN func)
{
    mouseMoveFunc = func;
}

void GLWidget::setMousePressFunc(PTRFUN func)
{
    mousePressFunc = func;
}

void GLWidget::setMouseReleaseFunc(PTRFUN func)
{
    mouseReleaseFunc = func;
}

void GLWidget::setKeyEventFunc(ACCEL_PTRFUN func)
{
    keyEventFunc = func;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (mouseMoveFunc)
        mouseMoveFunc(event);

    QOpenGLWidget::mouseMoveEvent(event);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (mousePressFunc)
        mousePressFunc(event);

    QOpenGLWidget::mousePressEvent(event);
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (mouseReleaseFunc)
        mouseReleaseFunc(event);

    QOpenGLWidget::mouseReleaseEvent(event);
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
    if (keyEventFunc)
        keyEventFunc(e);

    QOpenGLWidget::keyPressEvent(e);
}

void GLWidget::keyReleaseEvent(QKeyEvent *e)
{
    if (keyEventFunc)
        keyEventFunc(e);

    QOpenGLWidget::keyReleaseEvent(e);
}

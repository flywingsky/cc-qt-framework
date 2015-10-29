
#include "glwidget.h"

#include <QTimer>
#include <base/CCDirector.h>
#include <platform/CCApplication.h>
#include <renderer/CCRenderer.h>
#include "QtGLViewImpl.h"
#include "editor/LogTool.h"

USING_NS_CC;

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , timer_(NULL)
{
}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    LOG_DEBUG("GLWidget::initializeGL");

    Application::getInstance()->initGLContextAttrs();

    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview)
    {
        glview = QtGLViewImpl::create(this);
        director->setOpenGLView(glview);
    }

    if(!Application::getInstance()->applicationDidFinishLaunching())
    {
        LOG_ERROR("Failed to init application!!!");
        return;
    }

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

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QOpenGLWidget::mouseMoveEvent(event);

    emit signalMouseEvent(event);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    QOpenGLWidget::mousePressEvent(event);

    emit signalMouseEvent(event);
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QOpenGLWidget::mouseReleaseEvent(event);

    emit signalMouseEvent(event);
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    QOpenGLWidget::keyPressEvent(event);

    emit signalKeyEvent(event);
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    QOpenGLWidget::keyReleaseEvent(event);

    emit signalKeyEvent(event);
}

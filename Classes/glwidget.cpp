
#include "glwidget.h"

#include <QTimer>
#include <QElapsedTimer>
#include <QOpenGLContext>

#include <base/CCDirector.h>
#include <platform/CCApplication.h>
#include <renderer/CCRenderer.h>
#include "QtGLViewImpl.h"
#include "editor/LogTool.h"

USING_NS_CC;

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , timer_(NULL)
    , isCocosInitialized_(false)
    , elapsedTimer_(nullptr)
{
}

GLWidget::~GLWidget()
{
    delete timer_;
    delete elapsedTimer_;
}

void GLWidget::initializeGL()
{
    LOG_DEBUG("GLWidget::initializeGL");

    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanupCocos);

    timer_ = new QTimer();
    connect(timer_, SIGNAL(timeout()), this, SLOT(update()));

    timer_->start(1000 / 60);

    elapsedTimer_ = new QElapsedTimer();
    elapsedTimer_->start();
}

void GLWidget::paintGL()
{
    //因为GLWidget::initializeGL函数中，默认的FrameBuffer还没有创建完成
    //cocos将无法获得正确的FrameBuffer，需要延迟到渲染这里。
    if(!isCocosInitialized_)
    {
        isCocosInitialized_ = true;
        initializeCocos();

    }

    double delta = elapsedTimer_->elapsed() * 0.001;
    emit signalTick((float)delta);

    Director::getInstance()->mainLoop();
    elapsedTimer_->restart();
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

    emit signalResize(width, height);
}

void GLWidget::initializeCocos()
{
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
}

// cleanup opengl resource here.
void GLWidget::cleanupCocos()
{
    makeCurrent();
    Director::getInstance()->end();
    Director::getInstance()->mainLoop();
    doneCurrent();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QOpenGLWidget::mouseMoveEvent(event);

    makeCurrent();
    emit signalMouseEvent(event);
    doneCurrent();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    QOpenGLWidget::mousePressEvent(event);

    makeCurrent();
    emit signalMouseEvent(event);
    doneCurrent();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QOpenGLWidget::mouseReleaseEvent(event);

    makeCurrent();
    emit signalMouseEvent(event);
    doneCurrent();
}

void GLWidget::wheelEvent(QWheelEvent * event)
{
    QOpenGLWidget::wheelEvent(event);

    makeCurrent();
    emit signalWheelEvent(event);
    doneCurrent();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    QOpenGLWidget::keyPressEvent(event);

    makeCurrent();
    emit signalKeyEvent(event);
    doneCurrent();
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    QOpenGLWidget::keyReleaseEvent(event);

    makeCurrent();
    emit signalKeyEvent(event);
    doneCurrent();
}

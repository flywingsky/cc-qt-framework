#ifndef  _GL_WIDGET_H_
#define  _GL_WIDGET_H_

#undef CursorShape

#include "platform/CCGL.h"

#ifdef glClearDepthf
#undef glClearDepthf
#endif

#ifdef glDepthRangef
#undef glDepthRangef
#endif

#include <QOpenGLWidget>

class QTimer;
class QElapsedTimer;

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int width, int height) override;

signals:
    void signalMouseEvent(QMouseEvent *event);
    void signalKeyEvent(QKeyEvent *event);
    void signalResize(float width, float height);
    void signalWheelEvent(QWheelEvent * event);
    void signalTick(float delta);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent * event);
    virtual void keyPressEvent(QKeyEvent *) override;
    virtual void keyReleaseEvent(QKeyEvent *) override;

private:
    void initializeCocos();
    void cleanupCocos();

    QTimer*         timer_;
    bool            isCocosInitialized_;
    QElapsedTimer*  elapsedTimer_;
};

#endif // _GL_WIDGET_H_

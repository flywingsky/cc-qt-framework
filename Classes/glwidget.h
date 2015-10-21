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

typedef void(*PTRFUN)(QMouseEvent *event);
typedef void(*ACCEL_PTRFUN)(QKeyEvent *event);

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int width, int height) override;

    void setMouseMoveFunc(PTRFUN func);
    void setMousePressFunc(PTRFUN func);
    void setMouseReleaseFunc(PTRFUN func);
    void setKeyEventFunc(ACCEL_PTRFUN func);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *) override;
    virtual void keyReleaseEvent(QKeyEvent *) override;

private:
    PTRFUN          mouseMoveFunc;
    PTRFUN          mousePressFunc;
    PTRFUN          mouseReleaseFunc;

    ACCEL_PTRFUN    keyEventFunc;
    QTimer*         timer_;
};

#endif // _GL_WIDGET_H_

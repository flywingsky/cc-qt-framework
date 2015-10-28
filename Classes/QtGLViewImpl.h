#ifndef __CC_EGLViewIMPL_QT_H__
#define __CC_EGLViewIMPL_QT_H__

#include "base/CCRef.h"
#include "platform/CCCommon.h"
#include "platform/CCGLView.h"
#include "glwidget.h"

NS_CC_BEGIN

class CC_DLL QtGLViewImpl : public GLView
{
public:
    static QtGLViewImpl* create(GLWidget* qt_window);

    bool windowShouldClose();
    void pollEvents();

    /* override functions */
    virtual bool isOpenGLReady() override;
    virtual void end() override;
    virtual void swapBuffers() override;
    virtual void setFrameSize(float width, float height) override;
    virtual void setIMEKeyboardState(bool bOpen) override;

    virtual id getCocoaWindow() { return 0; }

protected:
    QtGLViewImpl();
    virtual ~QtGLViewImpl();

    bool initWithRect(GLWidget* qt_window, Rect rect, float frameZoomFactor);

    void updateFrameSize();
    bool initGlew();

    bool _captured;
    bool _supportTouch;

    float _mouseX;
    float _mouseY;
    GLWidget* window_;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(QtGLViewImpl);
};

NS_CC_END   // end of namespace   cocos2d

#endif  // end of __CC_EGLViewImpl_QT_H__

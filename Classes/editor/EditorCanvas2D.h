#ifndef CANVAS2D_H
#define CANVAS2D_H

#include "EditorCanvas.h"

namespace Editor
{

    class Canvas2D : public Canvas
    {
        Q_OBJECT
    public:
        Canvas2D(QObject *parent, GLWidget *view);

    public slots:
        virtual void onPopertyChange(PropertyParam &param) override;
        virtual void onMouseEvent(QMouseEvent *event) override;
        virtual void onKeyEvent(QKeyEvent *event) override;

    private:
        virtual void drawSelectedRect() override;

        void drawRect(const cocos2d::Point & pt, const cocos2d::Size & size,
                      const cocos2d::AffineTransform & world, const cocos2d::Color4F & color);

        void onNodeTouchMove(const cocos2d::Point & pt, const cocos2d::Point & old);

        void doNodeSelect(const cocos2d::Point & pt);
        void doNodeDrag(const cocos2d::Point & delta);
        void doNodeResize(const cocos2d::Point & delta);
        void doNodeScale(const cocos2d::Point & delta);
        void doNodeRotate(const cocos2d::Point & delta);

        bool handleDragEvent(QKeyEvent *event);


        cocos2d::RefPtr<cocos2d::DrawNode>  drawRect_;

        int                 dragMode_;
    };

} //end namespace Editor

#endif // CANVAS2D_H

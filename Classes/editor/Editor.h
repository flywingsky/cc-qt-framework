#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include <Singleton.h>

#include <platform/CCPlatformMacros.h>
#include <base/CCRefPtr.h>
#include <rapidjson/document.h>

#include <map>

// pre defination
class QtProperty;
class QtVariantEditorFactory;
class QtTreePropertyBrowser;

NS_CC_BEGIN
class Node;
NS_CC_END

namespace Editor
{

    typedef cocos2d::RefPtr<cocos2d::Node> NodePtr;

    class Editor : public QObject, public Singleton<Editor>
    {
        Q_OBJECT
    public:
        explicit Editor(QObject *parent = 0);
        ~Editor();

        bool init();

        void testProperty();

        void setRootNode(cocos2d::Node *root);
        void setTargetNode(cocos2d::Node *target);
    private:

    signals:

    public slots:
        void onPropertyChange(QtProperty *property, const QVariant &value);

    private:
        NodePtr         rootNode_;
        NodePtr         targetNode_;

        QtVariantEditorFactory*     editorFactory_;
        QtTreePropertyBrowser*      propertyTree_;

        rapidjson::Document         document_;
    };

}// end namespace Editor

#endif // EDITOR_H

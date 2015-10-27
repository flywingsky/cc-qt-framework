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

        void createNode(cocos2d::Node *node);

        bool saveLayout(const std::string & fileName);
        bool loadLayout(const std::string & fileName);
        void clearLayout();

    private:
        bool loadNodeConfigure(cocos2d::Node *node, const rapidjson::Value &value);
        bool saveNodeConfigure(cocos2d::Node *node, rapidjson::Value &value);

    signals:

    public slots:
        void onPropertyChange(QtProperty *property, const QVariant &value);

    private:
        NodePtr         rootNode_;
        NodePtr         targetNode_;

        QtVariantEditorFactory*     editorFactory_;
        QtTreePropertyBrowser*      propertyTree_;

        rapidjson::Document         document_;
        typedef std::map<cocos2d::Node*, rapidjson::Value> ConfigureMap;
        ConfigureMap                configures_;
        rapidjson::Value*           targetConfig_;
    };

}// end namespace Editor

#endif // EDITOR_H

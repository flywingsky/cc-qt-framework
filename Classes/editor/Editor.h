#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>

#include <base/CCRefPtr.h>
#include <rapidjson/document.h>

#include <map>

// predefination
class PropertyParam;

NS_CC_BEGIN
class Node;
NS_CC_END

namespace Editor
{
    typedef cocos2d::RefPtr<cocos2d::Node> NodePtr;

    class Hierarchy;
    class Canvas;
    class Inspector;

    class EditorScene;

    class Editor : public QObject
    {
        Q_OBJECT
    public:
        static Editor* instance(){ return s_instance; }

        explicit Editor(QObject *parent = 0);
        ~Editor();

        bool init();

        void createNode(cocos2d::Node *node);

        bool saveLayout(const std::string & fileName);
        bool loadLayout(const std::string & fileName);
        void clearLayout();

        rapidjson::Value::AllocatorType & getAllocator(){ return document_.GetAllocator(); }
        EditorScene* getScene(){ return scene_; }
        cocos2d::Node* getRootNode(){ return rootNode_; }
        cocos2d::Node* getTargetNode(){ return targetNode_; }
        rapidjson::Value* getTargetConfig(){ return targetConfig_; }

        void emitTargetPropertyChange(const std::string &name, const rapidjson::Value &value);

    private:
        void setRootNode(cocos2d::Node *root);

        bool loadNodeConfigure(cocos2d::Node *node, const rapidjson::Value &value);
        bool saveNodeConfigure(cocos2d::Node *node, rapidjson::Value &value);

    signals:
        void signalRootSet(cocos2d::Node *root);
        void signalTargetSet(cocos2d::Node *target);
        void signalNodeCreate(cocos2d::Node *node);
        void signalNodeDelete(cocos2d::Node *node);

    public slots:
        void setTargetNode(cocos2d::Node *target);

    public: // public component
        Hierarchy*      hierarchy_;
        Canvas*         canvas_;
        Inspector*      inspector_;

    private:
        NodePtr         rootNode_;
        NodePtr         targetNode_;
        cocos2d::RefPtr<EditorScene>     scene_;

        rapidjson::Document         document_;

        typedef std::map<cocos2d::Node*, rapidjson::Value> ConfigureMap;
        ConfigureMap                configures_;
        rapidjson::Value*           targetConfig_;


        static Editor* s_instance;
    };

}// end namespace Editor

#endif // EDITOR_H

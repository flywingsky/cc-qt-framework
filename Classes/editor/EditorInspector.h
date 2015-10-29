#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <QObject>
#include <base/CCRefPtr.h>
#include <rapidjson/document.h>

//predefination
NS_CC_BEGIN
class Node;
NS_CC_END

class QDockWidget;

class QtProperty;
class QtVariantEditorFactory;
class QtTreePropertyBrowser;

class PropertyParam;

namespace Editor
{
    class PropertyTreeNode;

    /** 属性面板。*/
    class Inspector : public QObject
    {
        Q_OBJECT
    public:
        explicit Inspector(QObject *parent, QDockWidget *view);

        void setTargetProperty(const std::string &name, const rapidjson::Value &value);
    signals:
        void signalPropertyChange(PropertyParam &param);

    public slots:
        void onPropertyChange(QtProperty *property, const QVariant &value);
        void onTargetSet(cocos2d::Node *target);
        void onPopertyChange(PropertyParam &param);

    private:
        void bindNameAndProperty();
        void applyDataToSheet();

        cocos2d::RefPtr<cocos2d::Node>      targetNode_;
        rapidjson::Value*                   targetConfig_;

        QtVariantEditorFactory*     editorFactory_;
        QtTreePropertyBrowser*      propertyTree_;

        typedef std::vector<PropertyTreeNode*> PropertieGroup;
        PropertieGroup              propertyGroup_;

        typedef std::map<std::string, QtProperty*> PropertyMap;
        PropertyMap                 name2property_;
    };

} // end namespace Editor

#endif // INSPECTOR_H

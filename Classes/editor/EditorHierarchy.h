#ifndef EDITOR_HIERARCHY_H
#define EDITOR_HIERARCHY_H

#include <QObject>
#include <base/CCRefPtr.h>
#include <map>

NS_CC_BEGIN
class Node;
NS_CC_END

class QTreeView;
class QStandardItem;
class QStandardItemModel;
class QModelIndex;

class PropertyParam;

namespace Editor
{
    typedef cocos2d::RefPtr<cocos2d::Node> NodePtr;

    class Hierarchy : public QObject
    {
        Q_OBJECT
    public:
        explicit Hierarchy(QObject *parent, QTreeView *treeView);


    signals:
        void signalSetTarget(cocos2d::Node *target);

    public slots:
        void onRootSet(cocos2d::Node *root);
        void onTargetSet(cocos2d::Node *target);
        void onNodeCreate(cocos2d::Node *node);
        void onNodeDelete(cocos2d::Node *node);
        void onPopertyChange(PropertyParam &param);

        void onTreeViewPressed(const QModelIndex &index);

    private:
        QStandardItem* loadNodeItem(cocos2d::Node *node);

        NodePtr         rootNode_;
        NodePtr         targetNode_;

        QTreeView*      treeView_;
        QStandardItemModel* itemModel_;

        std::map<cocos2d::Node*, QStandardItem*> node2item_;
    };

} // end namespace Editor

#endif // EDITOR_HIERARCHY_H

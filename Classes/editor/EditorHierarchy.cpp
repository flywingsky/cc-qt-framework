#include "EditorHierarchy.h"

#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>

#include <2d/CCNode.h>

namespace Editor
{
    Hierarchy::Hierarchy(QObject *parent, QTreeView *treeView)
        : QObject(parent)
        , treeView_(treeView)
        , itemModel_(nullptr)
    {
        itemModel_ = new QStandardItemModel(0, 1, this);
        treeView_->setModel(itemModel_);
    }

    void Hierarchy::onRootSet(cocos2d::Node *root)
    {
        rootNode_ = root;

        itemModel_->clear();
        node2item_.clear();

        if(rootNode_)
        {
            QStandardItem *item = loadNodeItem(root);
            itemModel_->appendRow(item);
        }
    }

    void Hierarchy::onTargetSet(cocos2d::Node *target)
    {
        targetNode_ = target;
    }

    void Hierarchy::onNodeCreate(cocos2d::Node *node)
    {
        if(node == rootNode_)
        {
            return;
        }

        auto it = node2item_.find(node->getParent());
        CCAssert(it != node2item_.end(), "Hierarchy::onNodeCreate: parent must be in cache.");

        QStandardItem *item = loadNodeItem(node);
        it->second->appendRow(item);
    }

    void Hierarchy::onNodeDelete(cocos2d::Node *node)
    {

    }

    void Hierarchy::onPopertyChange(PropertyParam &param)
    {

    }

    QStandardItem* Hierarchy::loadNodeItem(cocos2d::Node *node)
    {
        QString name(node->getName().empty() ? "no-name" : node->getName().c_str());
        QStandardItem *item = new QStandardItem(name);
        node2item_[node] = item;

        auto & children = node->getChildren();
        if(!children.empty())
        {
            QList<QStandardItem*> subItems;
            for(auto child : children)
            {
                QStandardItem *subItem = loadNodeItem(child);
                subItems.append(subItem);
            }
            item->appendRow(subItems);
        }

        return item;
    }

} // end namespace Editor

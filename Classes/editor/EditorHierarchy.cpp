#include "EditorHierarchy.h"

#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>

#include <2d/CCNode.h>

#include "uiloader/BaseLoader.h"
#include "LogTool.h"

Q_DECLARE_METATYPE(cocos2d::Node*)

namespace Editor
{
    namespace
    {
        QString name2title(const std::string &name)
        {
            return QString(name.empty() ? "no-name" : name.c_str());
        }
    }

    Hierarchy::Hierarchy(QObject *parent, QTreeView *treeView)
        : QObject(parent)
        , treeView_(treeView)
        , itemModel_(nullptr)
    {
        itemModel_ = new QStandardItemModel(0, 1, this);
        treeView_->setModel(itemModel_);

        connect(treeView_, SIGNAL(pressed(QModelIndex)), SLOT(onTreeViewPressed(QModelIndex)));

        QAbstractItemDelegate *delegate = treeView_->itemDelegate();
        LOG_DEBUG("deleget exist : %s", delegate ? "true" : "false");
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
        if(target)
        {
            auto it = node2item_.find(target);
            CCAssert(it != node2item_.end(), "");

            //QStandardItem *item = it->second;
        }
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
        auto it = node2item_.find(node);
        CCAssert(it != node2item_.end(), "");

        QStandardItem *item = it->second;
        delete item;

        node2item_.erase(it);
    }

    void Hierarchy::onPopertyChange(PropertyParam &param)
    {
        if(param.name == "name")
        {
            QStandardItem *item = node2item_.find(param.node)->second;
            item->setText(name2title(param.node->getName()));
        }
    }

    QStandardItem* Hierarchy::loadNodeItem(cocos2d::Node *node)
    {
        QStandardItem *item = new QStandardItem(name2title(node->getName()));
        item->setData(QVariant::fromValue(node), Qt::UserRole + 1);
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
            item->appendRows(subItems);
        }

        return item;
    }


    void Hierarchy::onTreeViewPressed(const QModelIndex &index)
    {
        cocos2d::Node *node = index.data(Qt::UserRole + 1).value<cocos2d::Node*>();
        emit signalSetTarget(node);
    }

} // end namespace Editor

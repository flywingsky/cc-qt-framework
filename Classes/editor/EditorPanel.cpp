//
//  EditorPanel.cpp
//  Clover
//
//  Created by C218-pc on 15/7/7.
//
//

#include "EditorPanel.h"
#include "EditorPropertyPanel.h"
#include "EditorHierarchy.h"
#include "EditorMenu.h"
#include "EditorCanvas.h"
#include "EditorPropertyMgr.h"
#include "EditorTools.h"
#include "EditorTooBox.h"
#include "FileTools.h"
#include "EditorStartup.h"
#include "EditorFileBrowser.h"
#include "EditorKeyEvent.h"
#include "EditorSettings.h"
#include "EditorPropertyUI.h"
#include "EditorProject.h"
#include "LogTool.h"
#include "EditorConsole.h"
#include "EditorPopupMenu.h"

#include <CCDirector.h>
#include <sprite_nodes/CCSprite.h>
#include <platform/CCFileUtils.h>
#include <text_input_node/CCIMEDispatcher.h>

#include <uilib/UILoader.h>
#include <uilib/UIHelper.h>

DEFINE_LOG_COMPONENT(LOG_PRIORITY_DEBUG, "Editor");
namespace Editor
{
    EditorPanel * g_editor = NULL;
    
    namespace
    {
        void cloneUIProperty(rapidjson::Value & output,
                             const rapidjson::Value & input,
                             rapidjson::Value::AllocatorType & allocator)
        {
            output.SetObject();
            
            // clone properties.
            for(rapidjson::Value::ConstMemberIterator it = input.MemberBegin();
                it != input.MemberEnd(); ++it)
            {
                if(strcmp(it->name.GetString(), "children") != 0)
                {
                    rapidjson::Value key, val;
                    clone_value(key, it->name, allocator);
                    clone_value(val, it->value, allocator);
                    output.AddMember(key, val, allocator);
                }
            }
        }
    }
    

    EditorPanel::EditorPanel()
        : m_document(NULL)
        , m_selectedConfig(NULL)
        , m_modified(false)
        , m_keyEventListener(NULL)
        , listener(this)
    {
        g_editor = this;
    }
    
    EditorPanel::~EditorPanel()
    {
        if(g_editor == this)
        {
            g_editor = NULL;
        }

        clearTargetConfigure();
        
        delete m_keyEventListener;
    }
    
    bool EditorPanel::init()
    {
        if(!uilib::Widget::init()) return false;
        
        if(!loadLayoutFromFile("editor/layout/editor_panel.json")) return false;
        
        if(!assignProperty(m_mainLayer, "mainLayer")) return false;
        if(!assignProperty(m_topLayer, "topLayer")) return false;
        if(!assignProperty(m_infoLayer, "infoLayer")) return false;
        
        cocos2d::CCDirector *director = cocos2d::CCDirector::sharedDirector();
        
        setPosition(director->getVisibleOrigin());
        setContentSize(director->getVisibleSize());
        
        m_keyEventListener = new KeyEventListener(false);
        m_keyEventListener->keyEventListener.set(this, (SEL_KeyEvent)&EditorPanel::onKeyEvent);
        addKeyEventListener(m_keyEventListener);
        
        canvas = CanvasPanel::create();
        if(!canvas)
        {
            LOG_ERROR("Failed to load component 'CanvasPanel'");
            return false;
        }
        m_mainLayer->addChild(canvas);
        
        
        property = PropertyPanel::create();
        if(!property)
        {
            LOG_ERROR("Failed to load component 'PropertyPanel'");
            return false;
        }
        m_mainLayer->addChild(property);
        
        hierarchy = HierarchyPanel::create();
        if(!hierarchy)
        {
            LOG_ERROR("Failed to load component 'HierarchyPanel'");
            return false;
        }
        m_mainLayer->addChild(hierarchy);
        
        mainMenu = MainMenu::create();
        if(!mainMenu)
        {
            LOG_ERROR("Failed to load component 'MainMenu'");
            return false;
        }
        m_mainLayer->addChild(mainMenu);
        
        
        toolBox = ToolBoxPanel::create();
        if(!toolBox)
        {
            LOG_ERROR("Failed to load componet 'toolBox'");
            return false;
        }
        m_mainLayer->addChild(toolBox);
        
        settings = SettingsPanel::create();
        if(!settings)
        {
            LOG_ERROR("Failed to load componet 'settings'");
            return false;
        }
        
        m_browser = FileBrowser::createWithLayout();
        if(!m_browser)
        {
            LOG_ERROR("Failed to load componet 'filebrowser'");
            return false;
        }
        m_browser->setDefaultPath(Project::instance()->getGlobalSettingPath());
        
        console = ConsolePanel::create();
        if(!console)
        {
            LOG_ERROR("Failed to load component 'console'");
            return false;
        }
        m_infoLayer->addChild(console);
        
        popupMenu = PopupMenu::create();
        if(!popupMenu)
        {
            LOG_ERROR("Failed to load component 'popupMenu'");
            return false;
        }
        
        return true;
    }
    
    bool EditorPanel::loadLayoutFile(const std::string & layoutFile)
    {
        clearLayout();
        
        if(!openJsonFile(layoutFile, m_document) || !m_document.IsObject())
        {
            LOG_ERROR("Failed to open layout file '%s'", layoutFile.c_str());
            m_document.SetNull();
            return false;
        }
        
        if(!UILoader::instance()->upgradeLayoutFile(m_document))
        {
            LOG_ERROR("Failed to upgrade layout file '%s'", layoutFile.c_str());
            m_document.SetNull();
            return false;
        }
        
        cocos2d::CCNode * pNode = UILoader::instance()->loadLayoutFromStream(m_document);
        if(pNode != NULL)
        {
            loadNodeConfigure(pNode, m_document);
        }
        
        m_layoutFile = layoutFile;
        setTarget(pNode);
        
        return pNode != NULL;
    }
    
    void EditorPanel::clearLayout()
    {
        m_document.SetNull();
        m_layoutFile.clear();
        m_modified = false;
        
        clearTargetConfigure();
        
        setTarget(NULL);
    }
    
    bool EditorPanel::saveLayoutAs(const std::string & layoutFile)
    {
        if(!m_target)
        {
            return false;
        }
        saveNodeConfigure(m_target, m_document);
        
        m_layoutFile = layoutFile;
        return UILoader::instance()->saveLayoutToFile(layoutFile, m_document);
    }
    
    void EditorPanel::setTarget(cocos2d::CCNode * target)
    {
        m_target = target;
        
        setSelectedNode(NULL);

        VariantVector argument(1);
        argument[0].setv(target);
        dispatcher.dispatchEvent(EE_SET_ROOT, argument);
        
        if(m_target)
        {
            setSelectedNode(m_target);
        }
        else
        {
            clearTargetConfigure();
        }
    }
    
    void EditorPanel::setSelectedNode(cocos2d::CCNode *selected)
    {
        m_selected = NULL;
        m_selectedConfig = NULL;
        
        if(selected != NULL && selected->getParent() != NULL)
        {
            //m_selected is only valid when the configure exist.
            m_selectedConfig = findNodeConfig(selected);
            if(m_selectedConfig != NULL)
            {
                m_selected = selected;
            }
        }
        
        rapidjson::Value nullObject;
        
        VariantVector argument(2);
        argument[0].setv(m_selected.get());
        argument[1].setv(m_selectedConfig ? m_selectedConfig : &nullObject);
        dispatcher.dispatchEvent(EE_SET_SELECT, argument);
    }
    
    void EditorPanel::notifyPropertyChange(cocos2d::CCNode* sender, const std::string & name, const rapidjson::Value & value)
    {
        CCAssert(m_selected && m_selectedConfig, "EditorPanel::setSelectedProperty");
        
        const std::string & type = PropertyMgr::instance()->cppNameToUIName(typeid(*m_selected).name());
        CCAssert(!type.empty(), "EditorPanel::notifyPropertyChange");
        
        IBaseLoader * loader = UILoader::instance()->findLoader(type);
        if(NULL == loader)
        {
            LOG_ERROR("Failed to find UI loader for type '%s'", type.c_str());
            return;
        }
        
        PropertyParam param(m_selected, name, value, (*m_selectedConfig)["property"], getAllocator());
        if(!loader->hookPropertyChange(param))
        {
            LOG_DEBUG("The property '%s' can't be set.", name.c_str());
            return;
        }
        
        if(sender != this)
        {
            applyUIPropertyChange(name, value);
        }
        
        VariantVector argument(3);
        argument[0].setv(sender);
        argument[1].setv(name);
        argument[2].setv(&value);
        dispatcher.dispatchEvent(EE_PROPERTY_CHANGE, argument);
    }
    
    void EditorPanel::storePropertyChange(const std::string & name, const rapidjson::Value & value, IPropertyUI *sender)
    {
        CCAssert(m_selectedConfig != NULL, "EditorPanel::storeSelectedProperty");
        
        rapidjson::Value * config = NULL;
        if(name == "skin" || name == "type")
        {
            config = m_selectedConfig;
        }
        else
        {
            config = &((*m_selectedConfig)["property"]);
        }
        CCAssert(config->IsObject(), "EditorPanel::storeSelectedProperty");
        
        m_modified = true;
        
        if(sender->isModified())
        {
            rapidjson::Value temp;
            clone_value(temp, value, m_document.GetAllocator());
            
            rapidjson::Value & slot = (*config)[name.c_str()];
            if(slot.IsNull())
            {
                config->AddMember(name.c_str(), m_document.GetAllocator(), temp, m_document.GetAllocator());
            }
            else
            {
                slot = temp;
            }
        }
        else
        {
            config->RemoveMemberStable(name.c_str());
        }
    }
    
    void EditorPanel::applyUIPropertyChange(const std::string & name, const rapidjson::Value & value)
    {
        if(hookUIPropertyChange(name, value))
        {
            return;
        }
        
        const std::string & type = PropertyMgr::instance()->cppNameToUIName(typeid(*m_selected).name());
        CCAssert(!type.empty(), "EditorPanel::setSelectedProperty");
        
        IBaseLoader * loader = UILoader::instance()->findLoader(type);
        if(NULL == loader)
        {
            LOG_ERROR("Failed to find UI loader for type '%s'", type.c_str());
            return;
        }
        
        rapidjson::Value & properties = (*m_selectedConfig)["property"];
        if(!loader->setProperty(m_selected, name, value, properties))
        {
            LOG_ERROR("Failed to find property '%s' for type '%s'",
                       name.c_str(), type.c_str());
            return;
        }
    }
    
    bool EditorPanel::hookUIPropertyChange(const std::string & name, const rapidjson::Value & value)
    {
        if(name == "touchEnable")
        {
        }
        else if(name == "skin")
        {
            if(value.IsString())
            {
                uilib::Widget *widget = dynamic_cast<uilib::Widget*>(m_selected.get());
                if(NULL != widget)
                {
                    widget->setSkin(value.GetString());
                }
            }
        }
        else if(name == "type")
        {
        }
        else if(name == "size")
        {
            //after changed position, we shuld change percent position.
            uilib::Widget * widget = dynamic_cast<uilib::Widget*>(m_selected.get());
            if(NULL != widget)
            {
                widget->absoluteSize2Relative();
                
                rapidjson::Value tempValue;
                tempValue.SetArray();
                tempValue.PushBack(widget->getPercentSize().width, getAllocator());
                tempValue.PushBack(widget->getPercentSize().height, getAllocator());
                notifyPropertyChange(this, "percentSize", tempValue);
            }
            return false;
        }
        else if(name == "position")
        {
            //after changed position, we shuld change percent position.
            uilib::Widget * widget = dynamic_cast<uilib::Widget*>(m_selected.get());
            if(NULL != widget)
            {
                widget->absolutePosition2Relative();
                
                rapidjson::Value tempValue;
                tempValue.SetArray();
                tempValue.PushBack(widget->getPercentPosition().x, getAllocator());
                tempValue.PushBack(widget->getPercentPosition().y, getAllocator());
                notifyPropertyChange(this, "percentPosition", tempValue);
            }
            return false;
        }
        else
        {
            return false;
        }
        
        return true;
    }

    void EditorPanel::createNode(const std::string & uiName, const std::string & uiSkin)
    {
        IBaseLoader * loader = UILoader::instance()->findLoader(uiName);
        if(NULL == loader)
        {
            LOG_ERROR("Failed to find UI loader for type '%s'", uiName.c_str());
            return;
        }
        
        if(!m_document.IsObject())
        {
            m_document.SetObject();
        }
        
        rapidjson::Value::AllocatorType & allocator = m_document.GetAllocator();
        
        rapidjson::Value config;
        config.SetObject();
        
        rapidjson::Value jtype(uiName.c_str(), allocator);
        config.AddMember("type", allocator, jtype, allocator);
        
        if(!uiSkin.empty())
        {
            rapidjson::Value jskin(uiSkin.c_str(), allocator);
            config.AddMember("skin", allocator, jskin, allocator);
        }
        
        rapidjson::Value jproperty(rapidjson::kObjectType);
        config.AddMember("property", allocator, jproperty, allocator);
        
        cocos2d::CCNode * node = loader->createObject(config);
        if(NULL == node)
        {
            LOG_ERROR("Failed to create node for type '%s'", uiName.c_str());
            return;
        }
        if(!uiSkin.empty())
        {
            uilib::Widget * widget = dynamic_cast<uilib::Widget*>(node);
            if(NULL != widget)
            {
                widget->setSkin(uiSkin);
            }
        }
        
        addNode(node, config);
        canvas->refreshTouchPriority();
        
        VariantVector argument(2);
        argument[0].setv(node);
        argument[1].setv(node->getParent());
        dispatcher.dispatchEvent(EE_CREATE_NODE, argument);
    }
    
    void EditorPanel::addNode(cocos2d::CCNode *node, rapidjson::Value & config)
    {
        CCAssert(node->getParent() == NULL, "EditorPanel::addNode");
        
        // add to configure map first.
        rapidjson::Value *tmp = new rapidjson::Value();
        *tmp = config;
        m_configureMap[node] = tmp;
        
        if(m_selected)
        {
            m_selected->addChild(node);
        }
        else if(m_target)
        {
            m_target->addChild(node);
        }
        else
        {
            setTarget(node);
        }
        
        node->getParent()->sortAllChildren();
        
        // update property panel.
        if(node == m_selected)
        {
            setSelectedNode(m_selected);
        }
    }
    
    void EditorPanel::removeNode(cocos2d::CCNode *node)
    {
        cocos2d::CCNode* nodeParent = node->getParent();
        
        if(node == m_selected)
        {
            setSelectedNode(NULL);
        }
        
        if(node == m_target)
        {
            setTarget(NULL);
        }
        else
        {
            node->removeFromParent();
        }
        
        // remove node configure.
        ConfigureMap::iterator it = m_configureMap.find(node);
        if(it != m_configureMap.end())
        {
            delete it->second;
            m_configureMap.erase(it);
        }
        
        VariantVector eventArg(2);
        eventArg[0].setv(node);
        eventArg[1].setv(nodeParent);
        dispatcher.dispatchEvent(EE_DELETE_NODE, eventArg);
    }

    rapidjson::Value * EditorPanel::findNodeConfig(cocos2d::CCNode *node)
    {
        ConfigureMap::iterator it = m_configureMap.find(node);
        if(it != m_configureMap.end())
        {
            return it->second;
        }
        return NULL;
    }
        
    FileBrowser * EditorPanel::showFileBrowser(bool show)
    {
        if(show)
        {
            if(m_browser->getParent() == NULL)
            {
                addPopupChild(m_browser);
            }
        }
        else
        {
            if(m_browser->getParent() != NULL)
            {
                removePopupChild(m_browser, false);
            }
        }
        return m_browser;
    }
    
    void EditorPanel::addPopupChild(cocos2d::CCNode *child)
    {
        CCAssert(child->getParent() == NULL, "EditorPanel::addPopupChild");
        
        if(!m_topLayer->isVisible())
        {
            m_topLayer->setVisible(true);
            
            // refresh touch priority
            m_topLayer->setTouchEnabled(false);
            m_topLayer->setTouchEnabled(true);
        }
        
        m_topLayer->addChild(child);
    }
    
    void EditorPanel::removePopupChild(cocos2d::CCNode *child, bool cleanup)
    {
        CCAssert(child->getParent() == m_topLayer, "EditorPanel::removePopupChild");
        
        m_topLayer->removeChild(child, cleanup);
        
        if(m_topLayer->getChildrenCount() == 0)
        {
            m_topLayer->setVisible(false);
        }
    }

    bool EditorPanel::startup()
    {
        StartupPanel * startupPanel = StartupPanel::create();
        if(NULL == startupPanel)
        {
            LOG_ERROR("Failed to create startup panel");
            return false;
        }
        addPopupChild(startupPanel);
        startupPanel->closeEventListener.set(this, (SEL_StartupPanelClose)&EditorPanel::onStartupPanelClose);
        
        return startupPanel->loadConfigure(Project::instance()->getGlobalSettingPath() + "config.json");
    }
    
    void EditorPanel::onStartupPanelClose(StartupPanel* sender, bool ok)
    {
        do
        {
            CC_BREAK_IF(!ok);

            CC_BREAK_IF(!Project::instance()->init(sender->getSelectedPath()));
            
            m_browser->setDefaultPath(Project::instance()->getResPath());
            removePopupChild(sender);
            return;
        }while(0);

        removePopupChild(sender);
        cocos2d::CCDirector::sharedDirector()->end();
        exit(0);
    }

    bool EditorPanel::onKeyEvent(const KeyEvent & event)
    {
        if(cocos2d::CCIMEDispatcher::sharedDispatcher()->existIMEDelegate())
            return true;
        
        return false;
    }
    
    void EditorPanel::clearTargetConfigure()
    {
        for(ConfigureMap::iterator it = m_configureMap.begin();
            it != m_configureMap.end(); ++it)
        {
            delete it->second;
        }
        m_configureMap.clear();
    }
    
    bool EditorPanel::loadNodeConfigure(cocos2d::CCNode* node, const rapidjson::Value & value)
    {
        CCAssert(value.IsObject(), "EditorPanel::loadNodeConfigure");
        
        rapidjson::Value *config = new rapidjson::Value();
        m_configureMap[node] = config;

        // clone properties.
        cloneUIProperty(*config, value, m_document.GetAllocator());
        
        
        // clone children configure.
        const rapidjson::Value & jchildren = value["children"];
        cocos2d::CCArray *children = node->getChildren();
        if(children != NULL && children->count() > 0 && jchildren.IsArray())
        {
            unsigned nChildren = std::min(children->count(), jchildren.Size());
            for(unsigned i = 0; i < nChildren; ++i)
            {
                const rapidjson::Value & jchild = jchildren[i];
                if(jchild.IsObject())
                {
                    cocos2d::CCNode* child = (cocos2d::CCNode*)children->objectAtIndex(i);
                    loadNodeConfigure(child, jchild);
                }
            }
        }
        
        return true;
    }
    
    bool EditorPanel::saveNodeConfigure(cocos2d::CCNode* node, rapidjson::Value & value)
    {
        CCAssert(node != NULL, "EditorPanel::saveNodeConfigure");
        
        const rapidjson::Value *config = m_configureMap[node];
        if(NULL == config)
        {
            return false;
        }
        
        // clone property
        cloneUIProperty(value, *config, m_document.GetAllocator());
        
        // clone children.
        cocos2d::CCArray *children = node->getChildren();
        if(children != NULL && children->count() > 0)
        {
            rapidjson::Value jchildren;
            jchildren.SetArray();
            jchildren.Reserve(children->count(), m_document.GetAllocator());
            
            for(unsigned i = 0; i < children->count(); ++i)
            {
                rapidjson::Value jchild;
                cocos2d::CCNode* child = (cocos2d::CCNode*)children->objectAtIndex(i);
                
                if(saveNodeConfigure(child, jchild))
                {
                    jchildren.PushBack(jchild, m_document.GetAllocator());
                }
            }
            
            value.AddMember("children", jchildren, m_document.GetAllocator());
        }
        
        return true;
    }

} // end namespace Editor

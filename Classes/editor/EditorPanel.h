//
//  EditorPanel.h
//  Clover
//
//  Created by C218-pc on 15/7/7.
//
//

#ifndef __Editor__EditorPanel__
#define __Editor__EditorPanel__

#include <uilib/UIWidget.h>
#include "CCRefPtr.h"
#include "EditorEventListener.h"

#include <rapidjson/document.h>

namespace Editor
{
    using cocos2d::RefPtr;
    
    class PropertyPanel;
    class HierarchyPanel;
    class MainMenu;
    class CanvasPanel;
    class ToolBoxPanel;
    class StartupPanel;
    class FileBrowser;
    class SettingsPanel;
    class ConsolePanel;
    class PopupMenu;
    
    class KeyEvent;
    class KeyEventListener;
    
    class IPropertyUI;
    
    enum EditorEvent
    {
        EE_SET_ROOT,    // args = (CCNode* node)
        EE_SET_SELECT,  // args = (CCNode* select, rapidjson::Value* config)
        EE_CREATE_NODE, // args = (CCNode* node, CCNode* parent)
        EE_DELETE_NODE, // args = (CCNode* node, CCNode* parent)
        EE_PROPERTY_CHANGE, // args = (CCNode* sender, string* name, rapidjson::Value* value)
    };
    
    class EditorPanel : public uilib::Widget
    {
    public:
        CREATE_FUNC(EditorPanel)
        
        EventDispatcher     dispatcher;
        EventListener       listener;
        
        EditorPanel();
        ~EditorPanel();
        
        virtual bool init() CC_OVERRIDE;
        
        bool loadLayoutFile(const std::string & layoutFile);
        bool saveLayoutAs(const std::string & layoutFile);
        void clearLayout();
        
        const std::string & getLayoutFile() const { return m_layoutFile; }
        bool isModified() const { return m_modified; }
        
        rapidjson::Document & getDocument(){ return m_document; }
        rapidjson::Document::AllocatorType & getAllocator(){ return m_document.GetAllocator(); }
        
        void setSelectedNode(cocos2d::CCNode *selected);
        void notifyPropertyChange(cocos2d::CCNode* sender, const std::string & name, const rapidjson::Value & value);
        void storePropertyChange(const std::string & name, const rapidjson::Value & value, IPropertyUI *sender);
        
        void createNode(const std::string & uiName, const std::string & uiSkin);
        void removeNode(cocos2d::CCNode *node);

        FileBrowser * showFileBrowser(bool show);

        void addPopupChild(cocos2d::CCNode *child);
        void removePopupChild(cocos2d::CCNode *child, bool cleanup=true);
        
        bool startup();
        
        bool onKeyEvent(const KeyEvent & event);
        
    public:
        RefPtr<PropertyPanel>      property;
        RefPtr<HierarchyPanel>     hierarchy;
        RefPtr<MainMenu>           mainMenu;
        RefPtr<CanvasPanel>        canvas;
        RefPtr<ToolBoxPanel>       toolBox;
        RefPtr<SettingsPanel>      settings;
        RefPtr<ConsolePanel>       console;
        RefPtr<PopupMenu>          popupMenu;
        
        RefPtr<uilib::Widget>      m_mainLayer;
        RefPtr<uilib::Widget>      m_topLayer;
        RefPtr<uilib::Widget>      m_infoLayer;
        
    private:
        void setTarget(cocos2d::CCNode * target);
        
        void addNode(cocos2d::CCNode *node, rapidjson::Value & config);

        rapidjson::Value * findNodeConfig(cocos2d::CCNode *node);

        void onStartupPanelClose(StartupPanel* sender, bool ok);
        
        void applyUIPropertyChange(const std::string & name, const rapidjson::Value & value);
        bool hookUIPropertyChange(const std::string & name, const rapidjson::Value & value);
        
        void clearTargetConfigure();
        bool loadNodeConfigure(cocos2d::CCNode* node, const rapidjson::Value & value);
        bool saveNodeConfigure(cocos2d::CCNode* node, rapidjson::Value & value);
        
        
        RefPtr<cocos2d::CCNode>    m_target;
        RefPtr<cocos2d::CCNode>    m_selected;

        std::string         m_layoutFile;
        rapidjson::Document m_document;
        rapidjson::Value*   m_selectedConfig;

        typedef std::map<cocos2d::CCNode*, rapidjson::Value*> ConfigureMap;
        ConfigureMap        m_configureMap;
        
        RefPtr<FileBrowser> m_browser;


        bool                m_modified;
        
        KeyEventListener*   m_keyEventListener;
    };

    extern EditorPanel * g_editor;
    
} // end namespace Editor

#endif /* defined(__Editor__EditorPanel__) */

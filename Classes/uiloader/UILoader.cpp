#include "UILoader.h"

#include <platform/CCFileUtils.h>
#include "UIWidget.h"
#include "UISkinMgr.h"
#include "UIHelper.h"

////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SINGLETON(UILoader);

UILoader::UILoader()
{
    loadDefaultLoaders(this);
}

UILoader::~UILoader()
{
    for(Loaders::iterator it = loaders_.begin(); it != loaders_.end(); ++it)
    {
        delete it->second;
    }
}

cocos2d::CCNode * UILoader::loadLayoutFromFile(const std::string & filename, cocos2d::CCNode *p)
{
    rapidjson::Document document;
    if(!openJsonFile(filename, document) || !document.IsObject())
    {
        return NULL;
    }
    
    if(!upgradeLayoutFile(document))
    {
        CCLOGERROR("Failed to upgrade layout file '%s'", filename.c_str());
        return NULL;
    }
    
    return loadLayoutFromStream(document, p);
}

cocos2d::CCNode * UILoader::loadLayoutFromStream(rapidjson::Value & config, cocos2d::CCNode *p)
{
    IBaseLoader * loader = choiceLoader(config);
    if(NULL == loader)
    {
        return NULL;
    }
    
    if(NULL == p)
    {
        p = loader->createObject(config["property"]);
        if(NULL == p)
        {
            CCLOGERROR("Failed to create object for type '%s'", config["type"].GetString());
            return NULL;
        }
    }
    
    // load skin first.
    loadSkin(config, p);
    
    // then load property.
    loadProperty(loader, config, p);
    loadChildren(config, p);
    return p;
}

bool UILoader::loadSkinFromFile(const std::string & skin, uilib::Widget *p)
{
    if(skin.size() > 5 && skin.compare(skin.size() - 5, 5, ".json") == 0)
    {
        rapidjson::Document document;
        if(openJsonFile(skin, document) && document.IsObject())
        {
            if(!upgradeLayoutFile(document))
            {
                CCLOGERROR("Failed to upgrade skin file '%s'", skin.c_str());
                return NULL;
            }
            
            return loadSkinFromStream(document, p);
        }
    }
    else
    {
        // the skin has been upgraded.
        rapidjson::Document *document = uilib::SkinMgr::instance()->getSkin(skin);
        if(NULL != document)
        {
            return loadSkinFromStream(*document, p);
        }
    }
    
    return false;
}

bool UILoader::loadSkinFromStream(rapidjson::Value & config, uilib::Widget *p)
{
    IBaseLoader * loader = choiceLoader(config);
    if(NULL == loader) return false;
    
    // load protected children first.
    loadProtectedChildren(config, p);
    p->onSkinLoad();
    
    // then load property.
    loadProperty(loader, config, p);
    return true;
}

IBaseLoader * UILoader::choiceLoader(rapidjson::Value &config)
{
    rapidjson::Value & type = config["type"];
    if(!type.IsString())
    {
        CCLOGERROR("Invalid node type, string type needed.");
        return NULL;
    }
    
    IBaseLoader * loader = findLoader(type.GetString());
    if(NULL == loader)
    {
        CCLOGERROR("Failed to find loader for type '%s'", type.GetString());
        return NULL;
    }
    
    return loader;
}

void UILoader::loadProperty(IBaseLoader* loader, rapidjson::Value & config, cocos2d::CCNode *p)
{
    rapidjson::Value & property = config["property"];
    if(property.IsObject())
    {
        for(rapidjson::Value::MemberIterator it = property.MemberBegin(); it != property.MemberEnd(); ++it)
        {
            if(!loader->setProperty(p, it->name.GetString(), it->value, property))
            {
                CCLOG("Loader '%s': ignored property '%s'.",
                      loader->getLoaderName(),
                      it->name.GetString());
            }
        }
    }
}

void UILoader::loadSkin(rapidjson::Value & config, cocos2d::CCNode *p)
{
    rapidjson::Value & skin = config["skin"];
    if(skin.IsString())
    {
        uilib::Widget * pWidget = dynamic_cast<uilib::Widget*>(p);
        if(pWidget)
        {
            pWidget->setSkin(skin.GetString());
        }
    }
}

void UILoader::loadChildren(rapidjson::Value & config, cocos2d::CCNode *p)
{
    rapidjson::Value & children = config["children"];
    if(children.IsArray())
    {
        for(rapidjson::SizeType i = 0; i < children.Size(); ++i)
        {
            cocos2d::CCNode *child = loadLayoutFromStream(children[i]);
            if(child != NULL)
            {
                p->addChild(child);
            }
        }
    }
}

void UILoader::loadProtectedChildren(rapidjson::Value & config, uilib::ProtectedNode *p)
{
    rapidjson::Value & children = config["children"];
    if(children.IsArray())
    {
        for(rapidjson::SizeType i = 0; i < children.Size(); ++i)
        {
            cocos2d::CCNode *child = loadLayoutFromStream(children[i]);
            if(child != NULL)
            {
                p->addProtectedChild(child);
            }
        }
    }
}

void UILoader::registerLoader(const std::string & type, IBaseLoader *loader)
{
    CCLOG("register loader: %s", type.c_str());
    
    Loaders::iterator it = loaders_.find(type);
    if(it != loaders_.end())
    {
        delete it->second;
        it->second = loader;
    }
    else
    {
        loaders_[type] = loader;
    }
}

IBaseLoader * UILoader::findLoader(const std::string & type)
{
    Loaders::iterator it = loaders_.find(type);
    if(it != loaders_.end())
    {
        return it->second;
    }
    
    return NULL;
}

bool UILoader::upgradeLayoutFile(rapidjson::Document &document)
{
    int version = 0;
    rapidjson::Value & jversion = document["version"];
    if(jversion.IsInt())
    {
        version = jversion.GetInt();
    }
    
    if(version < IBaseLoader::Version)
    {
        if(!upgradeConfig(document, document.GetAllocator()))
        {
           return false;
        }
        
        version = IBaseLoader::Version;

        if(!jversion.IsNull())
        {
            jversion.SetInt(version);
        }
        else
        {
            document.AddMember("version", version, document.GetAllocator());
        }
    }
    return true;
}

bool UILoader::upgradeConfig(rapidjson::Value &config, rapidjson::Value::AllocatorType &allocator)
{
    IBaseLoader *loader = choiceLoader(config);
    if(NULL == loader)
    {
        return false;
    }
    
    rapidjson::Value &property = config["property"];
    if(property.IsObject() && !loader->upgradeProperty(property, allocator))
    {
        return false;
    }
    
    rapidjson::Value &jchildren = config["children"];
    if(jchildren.IsArray())
    {
        for(rapidjson::Value::ValueIterator it = jchildren.Begin();
            it != jchildren.End(); ++it)
        {
            if(!upgradeConfig(*it, allocator))
            {
                return false;
            }
        }
    }
    
    return true;
}

bool UILoader::saveLayoutToFile(const std::string & filename, rapidjson::Document & document)
{
    trimConfig(document, document.GetAllocator());
    if(!saveJsonFile(filename, document))
    {
        CCLOGERROR("Failed to save layout to file '%s'", filename.c_str());
        return false;
    }
    return true;
}

bool UILoader::trimConfig(rapidjson::Value &config, rapidjson::Value::AllocatorType &allocator)
{
    IBaseLoader *loader = choiceLoader(config);
    if(NULL == loader)
    {
        return false;
    }
    
    rapidjson::Value &property = config["property"];
    if(property.IsObject())
    {
        loader->trimProperty(property, allocator);
    }
    
    rapidjson::Value &jchildren = config["children"];
    if(jchildren.IsArray())
    {
        if(jchildren.Size() == 0)
        {
            config.RemoveMemberStable("children");
        }
        else
        {
            for(rapidjson::Value::ValueIterator it = jchildren.Begin();
                it != jchildren.End(); ++it)
            {
                if(!trimConfig(*it, allocator))
                {
                    return false;
                }
            }
        }
    }
    
    return true;
}

#include "UILoader.h"

#include <platform/CCFileUtils.h>
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

cocos2d::Node * UILoader::loadLayoutFromFile(const std::string & filename, cocos2d::Node *p)
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

cocos2d::Node * UILoader::loadLayoutFromStream(rapidjson::Value & config, cocos2d::Node *p)
{
    IBaseLoader * loader = choiceLoader(config);
    if(NULL == loader)
    {
        return NULL;
    }
    
    if(NULL == p)
    {
        p = loader->createObject(config);
        if(NULL == p)
        {
            CCLOGERROR("Failed to create object for type '%s'", config["type"].GetString());
            return NULL;
        }
    }
    
    // then load property.
    loadProperty(loader, config, p);
    loadChildren(config, p);
    return p;
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

void UILoader::loadProperty(IBaseLoader* loader, rapidjson::Value & config, cocos2d::Node *p)
{
    for(rapidjson::Value::ConstMemberIterator it = config.MemberBegin(); it != config.MemberEnd(); ++it)
    {
        std::string name = it->name.GetString();
        if(name == "children" || name == "type" || name == "version")
        {
            continue;
        }

        PropertyParam param(p, name, it->value, config);
        if(!loader->setProperty(param))
        {
            CCLOG("Loader '%s': ignored property '%s'.",
                  loader->getLoaderName(),
                  it->name.GetString());
        }
    }
}

void UILoader::loadChildren(rapidjson::Value & config, cocos2d::Node *p)
{
    rapidjson::Value & children = config["children"];
    if(children.IsArray())
    {
        for(rapidjson::SizeType i = 0; i < children.Size(); ++i)
        {
            cocos2d::Node *child = loadLayoutFromStream(children[i]);
            if(child != NULL)
            {
                p->addChild(child);
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
    
    if(!loader->upgradeProperty(config, allocator))
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
    if(!document.HasMember("version"))
    {
        document.AddMember("version", IBaseLoader::Version, document.GetAllocator());
    }

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
    
    loader->trimProperty(config, allocator);
    
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

#ifndef H_CLOVER_UILIB_H
#define H_CLOVER_UILIB_H

#include <map>

#include "BaseLoader.h"
#include "Singleton.h"


class UILoader : public Singleton<UILoader>
{
public:
    UILoader();
    ~UILoader();
    
    bool upgradeLayoutFile(rapidjson::Document & document);
    
    bool saveLayoutToFile(const std::string & filename, rapidjson::Document & document);
    
    cocos2d::Node * loadLayoutFromFile(const std::string & filename, cocos2d::Node *p=NULL);
    cocos2d::Node * loadLayoutFromStream(rapidjson::Value & config, cocos2d::Node *p=NULL);
    
    void registerLoader(const std::string & type, IBaseLoader *loader);
    IBaseLoader * findLoader(const std::string & type);
    
private:
    IBaseLoader * choiceLoader(rapidjson::Value & config);
    void loadProperty(IBaseLoader* loader, rapidjson::Value & config, cocos2d::Node *p);
    void loadChildren(rapidjson::Value & config, cocos2d::Node *p);
    
    bool upgradeConfig(rapidjson::Value &config, rapidjson::Value::AllocatorType &allocator);
    bool trimConfig(rapidjson::Value &config, rapidjson::Value::AllocatorType &allocator);
    
    typedef std::map<std::string, IBaseLoader*> Loaders;
    Loaders         loaders_;
};

void loadDefaultLoaders(UILoader *ld);

#endif //H_CLOVER_UILIB_H

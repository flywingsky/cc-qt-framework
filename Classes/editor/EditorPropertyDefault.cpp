#include "EditorPropertyDefault.h"
#include "uiloader/UIHelper.h"
#include "LogTool.h"

IMPLEMENT_SINGLETON(Editor::PropertyDefault);

namespace Editor
{

    PropertyDefault::PropertyDefault()
    {

    }

    PropertyDefault::~PropertyDefault()
    {
        for(rapidjson::Document *doc : documents_)
        {
            delete doc;
        }
    }

    bool PropertyDefault::loadDefault(const std::string &fileName)
    {
        rapidjson::Document *doc = new rapidjson::Document();
        if(!openJsonFile(fileName, *doc) || !doc->IsObject())
        {
            delete doc;
            return false;
        }

        for(rapidjson::Value::MemberIterator it = doc->MemberBegin();
            it != doc->MemberEnd(); ++it)
        {
            if(it->name.IsString() && it->value.IsObject())
            {
                std::string type = it->name.GetString();
                valueMap_[type] = &it->value;
            }
        }

        documents_.push_back(doc);
        return true;
    }

    const rapidjson::Value* PropertyDefault::name2config(const std::string &name) const
    {
        auto it = valueMap_.find(name);
        if(it != valueMap_.end())
        {
            return it->second;
        }

        return nullptr;
    }

} // end namespace Editor

#ifndef EDITORPROPERTYDEFAULT_H
#define EDITORPROPERTYDEFAULT_H

#include "Singleton.h"
#include <rapidjson/document.h>

#include <vector>
#include <map>

namespace Editor
{

    class PropertyDefault : public Singleton<PropertyDefault>
    {
    public:
        PropertyDefault();
        ~PropertyDefault();

        bool loadDefault(const std::string &fileName);

        const rapidjson::Value* name2config(const std::string &name) const;

    private:
        std::vector<rapidjson::Document*> documents_;

        typedef std::map<std::string, const rapidjson::Value*> ValueMap;
        ValueMap        valueMap_;
    };

} // end namespace Editor

#endif // EDITORPROPERTYDEFAULT_H

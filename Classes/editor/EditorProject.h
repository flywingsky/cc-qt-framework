//
//  EditorProject.h
//  Editor
//
//  Created by C218-pc on 15/8/11.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__EditorProject__
#define __Editor__EditorProject__

#include "Singleton.h"

#include <rapidjson/document.h>
#include <string>

namespace Editor
{
    class Project : public Singleton<Project>
    {
    public:
        Project();
        ~Project();
        
        /**
         *  projectPath can be a path or project files.
         */
        bool init(const std::string & projectPath);
        
        bool saveConfigure();
        
        /**
         *  project path is used to save current project settings.
         */
        const std::string & getProjectPath() const { return m_projectPath; }
        
        /**
         *  res path is the major resource path, all the resources to be saved are relative to this path.
         */
        const std::string & getResPath() const { return m_resPath; }
        
        /**
         *  globa setting path is a shared path to save editor global settings.
         */
        const std::string & getGlobalSettingPath() const { return m_globalSettingPath; }
        
        bool resolveToRelativePath(std::string & path);
        bool resolveToOfficialPath(std::string & path);
        bool resolveToFullPath(std::string & path);
        
        bool upgradeLayoutFiles();
        
    private:
        bool initSettingPath();
        bool loadSearchPaths(rapidjson::Document &document);
        
        bool upgradeLayoutFilesInPath(const std::string & path);
        
        int             m_version;
        std::string     m_resPath;
        std::string     m_projectPath;
        std::string     m_toolboxConfigure;
        
        std::string     m_globalSettingPath;
        
        std::string     m_configureFilePath;
    };
}

#endif /* defined(__Editor__EditorProject__) */

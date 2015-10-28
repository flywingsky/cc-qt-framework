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
        const std::string & getProjectPath() const { return projectPath_; }
        
        /**
         *  res path is the major resource path, all the resources to be saved are relative to this path.
         */
        const std::string & getResPath() const { return resPath_; }
        
        /**
         *  globa setting path is a shared path to save editor global settings.
         */
        const std::string & getGlobalSettingPath() const { return globalSettingPath_; }
        
        bool resolveToRelativePath(std::string & path);
        bool resolveToOfficialPath(std::string & path);
        bool resolveToFullPath(std::string & path);
        
        bool upgradeLayoutFiles();
        
    private:
        bool initSettingPath();
        bool loadSearchPaths(rapidjson::Document &document);
        
        bool upgradeLayoutFilesInPath(const std::string & path);
        
        int             version_;
        std::string     resPath_;
        std::string     projectPath_;
        std::string     toolboxConfigure_;
        
        std::string     globalSettingPath_;
        
        std::string     configureFilePath_;
    };
}

#endif /* defined(__Editor__EditorProject__) */

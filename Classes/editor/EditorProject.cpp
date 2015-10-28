//
//  EditorProject.cpp
//  Editor
//
//  Created by C218-pc on 15/8/11.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "EditorProject.h"

#include <platform/CCCommon.h>
#include <platform/CCFileUtils.h>

#include <uilib/UIHelper.h>
#include <uilib/UIMgr.h>
#include <uilib/UILoader.h>

#include "FileTools.h"
#include "EditorPanel.h"
#include "EditorTooBox.h"
#include "EditorSettings.h"
#include "LogTool.h"

#include <sys/stat.h>
#include <sstream>

DEFINE_LOG_COMPONENT(LOG_PRIORITY_DEBUG, "Project");
IMPLEMENT_SINGLETON(Editor::Project);

namespace Editor
{
    Project::Project()
    {
        initSettingPath();
    }
    
    Project::~Project()
    {
        
    }
    
    bool Project::init(const std::string & projectPath)
    {
        if(isFile(projectPath))
        {
            configureFilePath_ = projectPath;
            projectPath_ = getFilePath(projectPath);
        }
        else
        {
            projectPath_ = projectPath;
            formatPath(projectPath_);
            configureFilePath_ = projectPath_ + "project.json";
        }
        
        std::ostringstream ssTitle;
        ssTitle << "Editor(" << projectPath_ << ")";
        setWindowTitle(ssTitle.str());
        
        rapidjson::Document document;
        if(!openJsonFile(configureFilePath_, document) || !document.IsObject())
        {
            LOG_ERROR("Failed to open project files '%s'.", configureFilePath_.c_str());
            return false;
        }
        
        rapidjson::Value *jvalue = &document["resPath"];
        if(jvalue->IsString())
        {
            resPath_ = jvalue->GetString();
            resolveToFullPath(resPath_);
            formatPath(resPath_);
        }
        else
        {
            resPath_ = projectPath_;
        }
        
        // add project path to search path
        std::vector<std::string> paths;
        paths.push_back(projectPath_);
        if(resPath_ != projectPath_)
        {
            paths.push_back(resPath_);
        }
        paths.push_back("");
        cocos2d::CCFileUtils::sharedFileUtils()->setSearchPaths(paths);
        
        
        jvalue = &document["settings"];
        if(jvalue->IsString())
        {
            std::string settingsFile = jvalue->GetString();
            resolveToFullPath(settingsFile);
            
            if(!g_editor->settings->loadSettings(settingsFile))
            {
                g_editor->settings->saveSettings();
            }
        }
        
        
        jvalue = &document["toolbox"];
        if(jvalue->IsString())
        {
            toolboxConfigure_ = jvalue->GetString();
            resolveToFullPath(toolboxConfigure_);
            
            g_editor->toolBox->loadToolbox(toolboxConfigure_);
        }
        
        
        jvalue = &document["guiConfig"];
        if(jvalue->IsString())
        {
            std::string guiConfig = jvalue->GetString();
            resolveToFullPath(guiConfig);
            uilib::GUIMgr::instance()->addConfigureFile(guiConfig);
        }
        
        return true;
    }
    
    bool Project::saveConfigure()
    {
        return false;
    }
    
    bool Project::initSettingPath()
    {
        const char * home = getenv("HOME");
        if(NULL == home)
        {
            LOG_ERROR("Failed to get user home directory");
            return false;
        }
        
        globalSettingPath_ = joinPath(home, "CloverEditor");
        if(!isDir(globalSettingPath_))
        {
            mkdir(globalSettingPath_.c_str(), S_IRWXU);
        }
        formatPath(globalSettingPath_);
        
        return true;
    }
    
    bool Project::resolveToRelativePath(std::string & path)
    {
        if(stringStartWith(path.c_str(), projectPath_.c_str()))
        {
            path = path.substr(projectPath_.size());
        }
        else if(stringStartWith(path.c_str(), resPath_.c_str()))
        {
            path = path.substr(resPath_.size());
        }
        return true;
    }
    
    bool Project::resolveToOfficialPath(std::string & path)
    {
        if(stringStartWith(path.c_str(), projectPath_.c_str()))
        {
            path = joinPath("${PROJECT}", path.substr(projectPath_.size()));
        }
        else if(stringStartWith(path.c_str(), resPath_.c_str()))
        {
            path = joinPath("${RESOURCE}", path.substr(resPath_.size()));
        }
        return true;
    }
    
    bool Project::resolveToFullPath(std::string & path)
    {
        if(stringStartWith(path.c_str(), "${PROJECT}"))
        {
            path = joinPath(projectPath_, path.substr(10));
        }
        else if(stringStartWith(path.c_str(), "${RESOURCE}"))
        {
            path = joinPath(resPath_, path.substr(11));
        }
        normalizePath(path);
        return true;
    }
    
    bool Project::upgradeLayoutFiles()
    {
        rapidjson::Document document;
        if(!openJsonFile(configureFilePath_, document) || !document.IsObject())
        {
            LOG_ERROR("Failed to open project files '%s'.", configureFilePath_.c_str());
            return false;
        }
        
        rapidjson::Value & jpaths = document["layoutPaths"];
        if(!jpaths.IsArray() || jpaths.Size() == 0)
        {
            LOG_ERROR("'layoutPaths' was invalid.");
            return false;
        }
        
        for(rapidjson::Value::ValueIterator it = jpaths.Begin();
            it != jpaths.End(); ++it)
        {
            if(it->IsString())
            {
                std::string path = it->GetString();
                resolveToFullPath(path);
                
                upgradeLayoutFilesInPath(path);
            }
        }
        return true;
    }
    
    bool Project::upgradeLayoutFilesInPath(const std::string & path)
    {
        LOG_DEBUG("upgrade layout in path: %s", path.c_str());
        
        StringVector files;
        if(!listDir(path, files))
        {
            LOG_ERROR("Failed to list files for path '%s'", path.c_str());
            return false;
        }
        
        for(StringVector::iterator it = files.begin(); it != files.end(); ++it)
        {
            if((*it)[0] == '.')
            {
                continue;
            }
            
            std::string fullPath = joinPath(path, *it);
            if(fullPath[fullPath.size() - 1] == '/')
            {
                upgradeLayoutFilesInPath(fullPath);
            }
            else if(getFileExt(fullPath) == "json")
            {
                rapidjson::Document document;
                if(openJsonFile(fullPath, document) && document.IsObject())
                {
                    if(UILoader::instance()->upgradeLayoutFile(document))
                    {
                        UILoader::instance()->saveLayoutToFile(fullPath, document);
                    }
                    else
                    {
                        LOG_ERROR("Failed to upgrade layout file '%s'", fullPath.c_str());
                    }
                }
                else
                {
                    LOG_ERROR("Failed to open layout file '%s'", fullPath.c_str());
                }
            }
        }
        return true;
    }
}

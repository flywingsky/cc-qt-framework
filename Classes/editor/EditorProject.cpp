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
            m_configureFilePath = projectPath;
            m_projectPath = getFilePath(projectPath);
        }
        else
        {
            m_projectPath = projectPath;
            formatPath(m_projectPath);
            m_configureFilePath = m_projectPath + "project.json";
        }
        
        std::ostringstream ssTitle;
        ssTitle << "Editor(" << m_projectPath << ")";
        setWindowTitle(ssTitle.str());
        
        rapidjson::Document document;
        if(!openJsonFile(m_configureFilePath, document) || !document.IsObject())
        {
            LOG_ERROR("Failed to open project files '%s'.", m_configureFilePath.c_str());
            return false;
        }
        
        rapidjson::Value *jvalue = &document["resPath"];
        if(jvalue->IsString())
        {
            m_resPath = jvalue->GetString();
            resolveToFullPath(m_resPath);
            formatPath(m_resPath);
        }
        else
        {
            m_resPath = m_projectPath;
        }
        
        // add project path to search path
        std::vector<std::string> paths;
        paths.push_back(m_projectPath);
        if(m_resPath != m_projectPath)
        {
            paths.push_back(m_resPath);
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
            m_toolboxConfigure = jvalue->GetString();
            resolveToFullPath(m_toolboxConfigure);
            
            g_editor->toolBox->loadToolbox(m_toolboxConfigure);
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
        
        m_globalSettingPath = joinPath(home, "CloverEditor");
        if(!isDir(m_globalSettingPath))
        {
            mkdir(m_globalSettingPath.c_str(), S_IRWXU);
        }
        formatPath(m_globalSettingPath);
        
        return true;
    }
    
    bool Project::resolveToRelativePath(std::string & path)
    {
        if(stringStartWith(path.c_str(), m_projectPath.c_str()))
        {
            path = path.substr(m_projectPath.size());
        }
        else if(stringStartWith(path.c_str(), m_resPath.c_str()))
        {
            path = path.substr(m_resPath.size());
        }
        return true;
    }
    
    bool Project::resolveToOfficialPath(std::string & path)
    {
        if(stringStartWith(path.c_str(), m_projectPath.c_str()))
        {
            path = joinPath("${PROJECT}", path.substr(m_projectPath.size()));
        }
        else if(stringStartWith(path.c_str(), m_resPath.c_str()))
        {
            path = joinPath("${RESOURCE}", path.substr(m_resPath.size()));
        }
        return true;
    }
    
    bool Project::resolveToFullPath(std::string & path)
    {
        if(stringStartWith(path.c_str(), "${PROJECT}"))
        {
            path = joinPath(m_projectPath, path.substr(10));
        }
        else if(stringStartWith(path.c_str(), "${RESOURCE}"))
        {
            path = joinPath(m_resPath, path.substr(11));
        }
        normalizePath(path);
        return true;
    }
    
    bool Project::upgradeLayoutFiles()
    {
        rapidjson::Document document;
        if(!openJsonFile(m_configureFilePath, document) || !document.IsObject())
        {
            LOG_ERROR("Failed to open project files '%s'.", m_configureFilePath.c_str());
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

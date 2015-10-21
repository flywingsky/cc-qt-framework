//
//  FileTools.h
//  Editor
//
//  Created by C218-pc on 15/7/16.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#ifndef __Editor__FileTools__
#define __Editor__FileTools__

#include <cstdlib>
#include <string>
#include <vector>

namespace Editor
{
	typedef std::vector<std::string> StringVector;

    const std::string empty_string;

    void formatSlash(std::string & path);
    void formatPath(std::string & path);
    void formatPathNoEndSlash(std::string & path);
    
    void appendPathSlash(std::string & path);
    void trimPathSlash(std::string & path);
    
    std::string getFileExt(const std::string & filename);
    void removeFileExt(std::string & name);

    std::string getFilePath(const std::string & filename);
    std::string getFileName(const std::string & filename);
    std::string joinPath(const std::string & a, const std::string & b);
    std::string getParentPath(const std::string & filename, int tier);
    
    bool stringStartWith(const char *src, const char *prefix);
    bool stringEndWith(const char *src, const char *postfix);
    
    void normalizePath(std::string & path);
    
    bool listDir(const std::string & path, StringVector & files);
    bool isFile(const std::string & path);
    bool isDir(const std::string & path);
    bool isFileExist(const std::string & path);
    
    std::string getAppResPath();
    std::string getAppRootPath();
    std::string getAppModulePath();
}

#endif /* defined(__Editor__FileTools__) */


#import <Cocoa/Cocoa.h>

#include "FileTools.h"

namespace Editor
{
    std::string getAppResPath()
    {
        std::string path = [[[NSBundle mainBundle] resourcePath] UTF8String];
        appendPathSlash(path);
        return path;
    }

    std::string getAppRootPath()
    {
        std::string path = [[[NSBundle mainBundle] bundlePath] UTF8String];
        appendPathSlash(path);
        return path;
    }

    std::string getAppModulePath()
    {
        std::string path = getAppRootPath();
        return getFilePath(path);
    }
}

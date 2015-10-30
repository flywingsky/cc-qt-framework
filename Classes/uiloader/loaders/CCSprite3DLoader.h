#ifndef CCSPRITE3DLOADER_H
#define CCSPRITE3DLOADER_H

#include "CCNodeLoader.h"

class CCSprite3DLoader : public CCNodeLoader
{
public:
    DECLARE_LOADER(CCSprite3DLoader)

    CCSprite3DLoader();

    virtual cocos2d::Node* createObject(rapidjson::Value & config) override;
    virtual bool setProperty(PropertyParam &pp) override;
};

#endif // CCSPRITE3DLOADER_H

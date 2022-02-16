#pragma once
#include <Engine/Script.h>

class SceneConvertScript :
    public Script
{
public:
    virtual void update();

    CLONE(SceneConvertScript);

public:
    SceneConvertScript();
    ~SceneConvertScript();
};


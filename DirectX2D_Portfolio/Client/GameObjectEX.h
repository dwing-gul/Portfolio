#pragma once
#include <Engine/GameObject.h>

class GameObjectEX :
    public GameObject
{
public :
    virtual void finalUpdate();

public :
    GameObjectEX();
    ~GameObjectEX();
};


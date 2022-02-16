#pragma once
#include <Engine/Camera.h>

class CameraEX :
    public Camera
{
public:
    virtual void finalUpdate();

public:
    CameraEX();
    ~CameraEX();
};


#pragma once
#include "Scene.h"
class Scene_Title :
    public Scene
{
public:
	virtual void Enter();
	virtual void Exit();

public:
	Scene_Title();
	~Scene_Title();
};


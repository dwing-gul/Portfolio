#pragma once
#include "Scene.h"

class Scene_Stage2 :
	public Scene
{
private:
	bool m_bUISetting;

public:
	virtual void update();

	virtual void Enter();
	virtual void Exit();

public:
	Scene_Stage2();
	~Scene_Stage2();
};


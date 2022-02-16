#include "pch.h"
#include "WallScript.h"
#include <Engine/GameObject.h>
#include <Engine/Collider2D.h>

WallScript::WallScript() :
	Script((UINT)SCRIPT_TYPE::WALLSCRIPT),
	m_wallType(WALL_TYPE::RIGHT)
{
}

WallScript::~WallScript()
{
}

void WallScript::start()
{
}

void WallScript::update()
{
}

void WallScript::lateUpdate()
{
}

void WallScript::SaveToScene(FILE* _file)
{
	SaveToFile(&m_wallType, _file);
}

void WallScript::LoadFromScene(FILE* _file)
{
	LoadFromFile(&m_wallType, _file);
}

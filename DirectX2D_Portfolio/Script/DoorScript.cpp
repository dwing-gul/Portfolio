#include "pch.h"
#include "DoorScript.h"
#include <Engine/SceneManager.h>

DoorScript::DoorScript() :
	Script((UINT)SCRIPT_TYPE::DOORSCRIPT),
	m_dest(nullptr),
	m_type(DOOR_TYPE::ENTERANCE)
{
}

DoorScript::~DoorScript()
{
}

void DoorScript::start()
{
	m_dest = SceneManager::GetInst()->FindObjByName(m_destName);
}

void DoorScript::update()
{
	if(nullptr != m_dest)
		m_destName = m_dest->GetName();
}

void DoorScript::lateUpdate()
{
}

void DoorScript::SaveToScene(FILE* _file)
{
	SaveToFile(&m_type, _file);
	SaveWStringToFile(m_destName, _file);
}

void DoorScript::LoadFromScene(FILE* _file)
{
	LoadFromFile(&m_type, _file);
	LoadWStringFromFile(m_destName, _file);
}

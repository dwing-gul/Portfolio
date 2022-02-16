#include "pch.h"
#include "Script.h"
#include "EventManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Prefab.h"

Script::Script(UINT _type) :
	Component(COMPONENT_TYPE::SCRIPT),
	m_scriptType(_type)
{
}

Script::~Script()
{
}

void Script::CreateObject(GameObject* _newObj, Vector3 _worldPos, int _layerIndex)
{
	_newObj->GetTransform()->SetLocalPos(_worldPos);

	tEvent evn = {};
	evn.type = EVENT_TYPE::CREATE_OBJECT;
	evn.lparam = (DWORD_PTR)_newObj;
	evn.wparam = (DWORD_PTR)_layerIndex;

	EventManager::GetInst()->AddEvent(evn);
}

void Script::CreateObject(Scene* _scene, GameObject* _newObj, Vector3 _worldPos, int _layerIndex)
{
	_newObj->GetTransform()->SetLocalPos(_worldPos);

	tEvent evn = {};
	evn.type = EVENT_TYPE::CREATE_OBJECT_BY_SCENE;
	evn.lparam = (DWORD_PTR)_scene;
	evn.wparam = (DWORD_PTR)_newObj;
	evn.eparam = (DWORD_PTR)_layerIndex;

	EventManager::GetInst()->AddEvent(evn);
}

void Script::DeleteObject(GameObject* _target)
{
	tEvent evn = {};
	evn.type = EVENT_TYPE::DELETE_OBJECT;
	evn.lparam = (DWORD_PTR)_target;

	EventManager::GetInst()->AddEvent(evn);
}

void Script::AddChild(GameObject* _parent, GameObject* _child)
{
	tEvent evn = {};
	evn.type = EVENT_TYPE::ADD_CHILD;
	evn.lparam = (DWORD_PTR)_parent;
	evn.wparam = (DWORD_PTR)_child;

	EventManager::GetInst()->AddEvent(evn);
}

void Script::DisconnectWithParent(GameObject* _child)
{
	tEvent evn = {};
	evn.type = EVENT_TYPE::DISCONNECT_PARENT;
	evn.lparam = (DWORD_PTR)_child;

	EventManager::GetInst()->AddEvent(evn);
}

void Script::Instantiate(Ptr<Prefab> _prefab, Vector3 _worldPos, int _layerIndex)
{
	CreateObject(_prefab->Instantiate(), _worldPos, _layerIndex);
}
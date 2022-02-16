#include "pch.h"
#include "CScript.h"
#include "CEventManager.h"

CScript::CScript(UINT _type) :
	CComponent(COMPONENT_TYPE::SCRIPT),
	m_scriptType(_type)
{
}

CScript::~CScript()
{
}

void CScript::CreateObject(CGameObject* _newObj, Vec3 _worldPos, int _layerIndex)
{
	_newObj->Transform()->SetLocalPos(_worldPos);

	tEvent evn = {};
	evn.type = EVENT_TYPE::CREATE_OBJECT;
	evn.lParam = (DWORD_PTR)_newObj;
	evn.wParam = _layerIndex;

	CEventManager::GetInst()->AddEvent(evn);
}

void CScript::CreateObject(CGameObject* _newObj, int _layerIndex)
{
	tEvent evn = {};
	evn.type = EVENT_TYPE::CREATE_OBJECT;
	evn.lParam = (DWORD_PTR)_newObj;
	evn.wParam = _layerIndex;

	CEventManager::GetInst()->AddEvent(evn);
}

void CScript::DeleteObject(CGameObject* _target)
{
	tEvent evn = {};
	evn.type = EVENT_TYPE::DELETE_OBJECT;
	evn.lParam = (DWORD_PTR)_target;

	CEventManager::GetInst()->AddEvent(evn);
}

void CScript::AddChild(CGameObject* _parent, CGameObject* _child)
{
	tEvent evn = {};
	evn.type = EVENT_TYPE::ADD_CHILD;
	evn.lParam = (DWORD_PTR)_parent;
	evn.wParam = (DWORD_PTR)_child;

	CEventManager::GetInst()->AddEvent(evn);
}

void CScript::AddChildByName(const wstring& _parentName, CGameObject* _child)
{
	tEvent evn = {};
	evn.type = EVENT_TYPE::ADD_CHILD_BY_NAME;
	evn.lParam = (DWORD_PTR)_parentName.data();
	evn.wParam = (DWORD_PTR)_child;

	CEventManager::GetInst()->AddEventLater(evn);
}

void CScript::DisconnectWithParent(CGameObject* _child)
{
	tEvent evn = {};
	evn.type = EVENT_TYPE::DISCONNECT_PARENT;
	evn.lParam = (DWORD_PTR)_child;

	CEventManager::GetInst()->AddEvent(evn);
}

void CScript::Instantiate(Ptr<CPrefab> _prefab, Vec3 _worldPos, int _layerIndex)
{
	CreateObject(_prefab->Instantiate(), _worldPos, _layerIndex);
}
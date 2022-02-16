#include "pch.h"
#include "SlotScript.h"
#include "PartyIconScript.h"
#include <Engine/Transform.h>
#include <Engine/SceneManager.h>

SlotScript::SlotScript() :
	Script((UINT)SCRIPT_TYPE::SLOTSCRIPT),
	m_owner(nullptr),
	m_position(0)
{
}

SlotScript::~SlotScript()
{
}

void SlotScript::start()
{
	if (Object() == SceneManager::GetInst()->FindObjByName(L"PlayerSlot4"))
	{
		m_position = 0;
	}
	else if (Object() == SceneManager::GetInst()->FindObjByName(L"PlayerSlot3"))
	{
		m_position = 1;
	}
	else if (Object() == SceneManager::GetInst()->FindObjByName(L"PlayerSlot2"))
	{
		m_position = 2;
	}
	else if (Object() == SceneManager::GetInst()->FindObjByName(L"PlayerSlot1"))
	{
		m_position = 3;
	}
}

void SlotScript::update()
{
	if (nullptr != m_owner)
	{
		Vector3 ownerPos = m_owner->GetTransform()->GetLocalPos();
		Vector3 pos = GetTransform()->GetLocalPos();
		Vector3 scale = GetTransform()->GetLocalScale();

		if (abs(ownerPos.x - pos.x) > scale.x / 2.f || abs(ownerPos.y - pos.y) > scale.y)
		{
			m_owner->SetSlot(nullptr);
			m_owner = nullptr;
			m_prefab = nullptr;
		}
	}
}

void SlotScript::lateUpdate()
{
}

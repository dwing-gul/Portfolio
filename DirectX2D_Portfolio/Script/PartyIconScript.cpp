#include "pch.h"
#include "PartyIconScript.h"
#include "SlotScript.h"
#include <Engine/GameObject.h>
#include <Engine/Transform.h>
#include <Engine/Collider2D.h>
#include <Engine/SceneManager.h>
#include <Engine/KeyManager.h>

PartyIconScript::PartyIconScript() :
	Script((UINT)SCRIPT_TYPE::PARTYICONSCRIPT),
	m_slot(nullptr)
{
}

PartyIconScript::~PartyIconScript()
{
}

void PartyIconScript::start()
{
	if (Object()->GetName() == L"CrusaderPartyIcon")
	{
		m_prefab = ResourceManager::GetInst()->Load<Prefab>(L"Crusader", L"prefab\\Crusader.prefab");
	}
	else if(Object()->GetName() == L"HighwaymanPartyIcon")
	{
		m_prefab = ResourceManager::GetInst()->Load<Prefab>(L"Highwayman", L"prefab\\Highwayman.prefab");
	}
	else if (Object()->GetName() == L"PlagueDoctorPartyIcon")
	{
		m_prefab = ResourceManager::GetInst()->Load<Prefab>(L"PlagueDoctor", L"prefab\\PlagueDoctor.prefab");
	}
	else if (Object()->GetName() == L"VestalPartyIcon")
	{
		m_prefab = ResourceManager::GetInst()->Load<Prefab>(L"Vestal", L"prefab\\Vestal.prefab");
	}

	m_originPos = GetTransform()->GetLocalPos();
}

void PartyIconScript::update()
{
	if (nullptr != m_slot)
	{
		GetTransform()->SetLocalPos(m_slot->GetTransform()->GetLocalPos());
	}

	DragNDrop(Object());
}

void PartyIconScript::lateUpdate()
{
	const vector<GameObject*>& vectorSlot = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::GENERAL);

	for (size_t i = 0; i < vectorSlot.size(); ++i)
	{
		const vector<Script*>& vectorScript = vectorSlot[i]->GetScripts();

		for (size_t j = 0; j < vectorScript.size(); ++j)
		{
			if (vectorScript[j]->GetScriptType() == (UINT)SCRIPT_TYPE::SLOTSCRIPT)
			{
				Matrix worldMatrix = vectorSlot[i]->GetCollider2D()->GetWorldMatrix();
				float xHalf = worldMatrix._11 / 2.f;
				float yHalf = worldMatrix._22 / 2.f;
				Vector4 slotArea = Vector4(worldMatrix._41 - xHalf, worldMatrix._41 + xHalf,
					worldMatrix._42 - yHalf, worldMatrix._42 + yHalf);

				Vector3 pos = GetTransform()->GetLocalPos();
				Vector3 scale = GetTransform()->GetLocalScale();

				if (slotArea.x < pos.x && slotArea.y > pos.x && slotArea.z < pos.y && slotArea.w > pos.y)
				{
					if (((SlotScript*)vectorScript[j])->GetOwner() == nullptr)
					{
						GetTransform()->SetLocalPos(vectorSlot[i]->GetTransform()->GetLocalPos());
						((SlotScript*)vectorScript[j])->SetPrefab(m_prefab);
						((SlotScript*)vectorScript[j])->SetOwner(this);
						m_slot = ((SlotScript*)vectorScript[j]);
					}
				}
			}
		}
	}

	if (KEY_AWAY(KEY::LBUTTON) && nullptr == m_slot)
	{
		GetTransform()->SetLocalPos(m_originPos);
	}
}

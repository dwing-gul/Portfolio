#include "pch.h"
#include "PlayerWaitingScript.h"
#include "PartyManager.h"
#include <Script/ScriptManager.h>
#include <Script/SlotScript.h>

PlayerWaitingScript::PlayerWaitingScript() :
	Script(-3),
	m_bInput(false)
{
}

PlayerWaitingScript::~PlayerWaitingScript()
{
}

void PlayerWaitingScript::update()
{
	const vector<Script*>& vectorScript = Object()->GetScripts();
	for (size_t i = 0; i < vectorScript.size(); ++i)
	{
		if (vectorScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::SLOTSCRIPT)
		{
			if (nullptr != ((SlotScript*)vectorScript[i])->GetOwner() && !m_bInput)
			{
				PartyManager::GetInst()->SetWaitingPlayer(((SlotScript*)vectorScript[i])->GetPrefab().Get());
				m_bInput = true;
			}
			else if (m_bInput)
			{
				vector<Prefab*>& vectorPlayerWaiting = PartyManager::GetInst()->GetWaitingPlayer();

				for (size_t j = 0; j < vectorPlayerWaiting.size(); ++j)
				{
					if (((SlotScript*)vectorScript[i])->GetSlotPosition() == j)
					{
						vectorPlayerWaiting[j] = ((SlotScript*)vectorScript[i])->GetPrefab().Get();
					}
				}
			}
		}
	}
}
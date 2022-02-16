#include "pch.h"
#include "CMonsterScriptGUI.h"

#include <Engine\CAnimator3D.h>

CMonsterScriptGUI::CMonsterScriptGUI():
	CScriptGUI(SCRIPT_TYPE::MONSTERSCRIPT)
{
	Activate();
}

CMonsterScriptGUI::~CMonsterScriptGUI()
{
}

void CMonsterScriptGUI::init()
{
	for (int i = 0; i < (UINT)MONSTER_ANIMCLIP::END; ++i)
	{
		m_AnimName[i] = i;
	}
}

void CMonsterScriptGUI::update()
{
	if (Start())
	{
		CMonsterScript* monsterScript = (CMonsterScript*)GetTarget()->GetScriptComponent((UINT)SCRIPT_TYPE::MONSTERSCRIPT);

		MONSTER_TYPE monsterType = monsterScript->GetMonsterType();

		ImGui::Text("AnimAction");
		int curAnimIndex = (int)GetTarget()->Animator3D()->GetCurClipIndex();

		const char* combo_preview_value = m_AnimName[curAnimIndex].c_str();
		if (ImGui::BeginCombo("##AnimAction", combo_preview_value))
		{
			
			for (UINT i = 0; i < (UINT)MONSTER_ANIMCLIP::END; ++i)
			{
				const bool is_selected = (curAnimIndex == i);
				if (ImGui::Selectable(m_AnimName[i].c_str(), is_selected))
				{
					curAnimIndex = i;
					monsterScript->SetAnimAction((MONSTER_ANIMCLIP)curAnimIndex, false);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		End();
	}
}

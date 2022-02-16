#include "pch.h"
#include "CMonsterCreatGUI.h"

CMonsterCreatGUI::CMonsterCreatGUI() :
	CScriptGUI(SCRIPT_TYPE::MONSTERCREATESCRIPT)
{
}

CMonsterCreatGUI::~CMonsterCreatGUI()
{
}

void CMonsterCreatGUI::init()
{
	MonsterName[0] = "Bat";
	MonsterName[1] = "Bear";
	MonsterName[2] = "Boar";
	MonsterName[3] = "Chicken";
	MonsterName[4] = "Dog";
	MonsterName[5] = "Wolf";
	MonsterName[6] = "None";
}

void CMonsterCreatGUI::update()
{
	if (Start())
	{
		CMonsterCreateScript* CreateScript = (CMonsterCreateScript*)GetTarget()->GetScriptComponent((UINT)SCRIPT_TYPE::MONSTERCREATESCRIPT);
		MONSTER_TYPE monsterType = CreateScript->GetMonsterType();
		string name = GetMonsterType(monsterType);

		const char* combo_preview_value = MonsterName[(int)monsterType].c_str();
		if (ImGui::BeginCombo("##MonsterType", combo_preview_value))
		{
			for (UINT i = 0; i < 7; ++i)
			{
				const bool is_selected = ((int)monsterType == i);
				if (ImGui::Selectable(MonsterName[i].c_str(), is_selected))
				{
					monsterType = (MONSTER_TYPE)i;
					CreateScript->SetMonsterType(monsterType);
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("MonsterCreat"))
		{
			CreateScript->MonsterCreate();
		}

		End();
	}
}

string CMonsterCreatGUI::GetMonsterType(MONSTER_TYPE _type)
{
	switch (_type)
	{
	case MONSTER_TYPE::MONSTER_BAT:
		return "Bat";
	case MONSTER_TYPE::MONSTER_BEAR:
		return "Bear";
	case MONSTER_TYPE::MONSTER_BOAR:
		return "Boar";
	case MONSTER_TYPE::MONSTER_CHICKEN:
		return "Chicken";
	case MONSTER_TYPE::MONSTER_DOG:
		return "Dog";
	case MONSTER_TYPE::MONSTER_WOLF:
		return "Wolf";
	case MONSTER_TYPE::END:
		return "None";
	default:
		return "";
	}
	return "";
}

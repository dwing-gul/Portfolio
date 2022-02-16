#include "pch.h"
#include "SkillButtonScript.h"
#include "PlayerScript.h"
#include "MonsterScript.h"
#include <Engine/GameObject.h>
#include <Engine/Collider2D.h>
#include <Engine/MeshRender.h>
#include <Engine/SceneManager.h>
#include <Engine/Texture.h>
#include <Engine/Material.h>

SkillButtonScript::SkillButtonScript() :
	Script((UINT)SCRIPT_TYPE::SKILLBUTTONSCRIPT),
	m_ownerScript(nullptr),
	m_position(-1),
	m_type(SKILL_TYPE::END),
	m_bPass(false),
	m_bMove(false),
	m_bPortrait(false),
	m_bSelect(false)
{
}

SkillButtonScript::~SkillButtonScript()
{
	
}

void SkillButtonScript::start()
{
	
}

void SkillButtonScript::update()
{
	if(nullptr != PlayerScript::m_selectPlayer && !(PlayerScript::m_selectPlayer->IsDead()))
		m_ownerName = PlayerScript::m_selectPlayer->GetName();
}

void SkillButtonScript::lateUpdate()
{
	SetOwnerByName(m_ownerName);
	
	if (m_bPass)
	{
		SetPassTexture();

		if (nullptr != m_ownerScript && m_ownerScript->IsBattle())
		{
			if (GetCollider2D()->IsObjectHovered() && IsMouseClicked())
			{
				m_ownerScript->SetTurnEnd(true);

				Ptr<Sound> pass = ResourceManager::GetInst()->Load<Sound>(L"Sound_Pass", L"sound\\general\\gen_char_pass.wav");
				pass->Play(1, 0.5f, true);

				vector<GameObject*> vectorPlayerClone;
				vector<GameObject*> vectorMonsterClone;
				const vector<GameObject*>& vectorPlayer = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);
				for (size_t i = 0; i < vectorPlayer.size(); ++i)
				{
					vectorPlayerClone.push_back(vectorPlayer[i]);
				}
				const vector<GameObject*>& vectorMonster = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::MONSTER);
				for (size_t i = 0; i < vectorMonster.size(); ++i)
				{
					vectorMonsterClone.push_back(vectorMonster[i]);
				}

				for (size_t i = 0; i < vectorPlayerClone.size(); ++i)
				{
					((PlayerScript*)vectorPlayerClone[i]->GetScripts()[0])->SetHealTarget(false);
				}

				for (size_t i = 0; i < vectorMonsterClone.size(); ++i)
				{
					((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->SetBoolTarget(false);
				}
			}
		}
	}
	else if (m_bMove)
	{
		SetMoveTexture();
	}
	else if (m_bPortrait)
	{
		SetPortrait();
	}
	else
	{
		SetSkillButtonTexture();

		if (GetCollider2D()->IsObjectHovered() && IsMouseClicked())
		{
			m_ownerScript->SetSkillType(m_type);

			vector<GameObject*> vectorPlayerClone;
			vector<GameObject*> vectorMonsterClone;
			const vector<GameObject*>& vectorPlayer = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::PLAYER);
			for (size_t i = 0; i < vectorPlayer.size(); ++i)
			{
				vectorPlayerClone.push_back(vectorPlayer[i]);
			}
			const vector<GameObject*>& vectorMonster = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::MONSTER);
			for (size_t i = 0; i < vectorMonster.size(); ++i)
			{
				vectorMonsterClone.push_back(vectorMonster[i]);
			}

			if (m_type == SKILL_TYPE::BATTLEFIELD_MEDICINE || m_type == SKILL_TYPE::DIVINE_GRACE || m_type == SKILL_TYPE::DIVINE_COMFORT)
			{
				for (size_t i = 0; i < vectorPlayerClone.size(); ++i)
				{
					((PlayerScript*)vectorPlayerClone[i]->GetScripts()[0])->SetHealTarget(true);
				}

				for (size_t i = 0; i < vectorMonsterClone.size(); ++i)
				{
					((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->SetBoolTarget(false);
				}
			}
			else
			{
				for (size_t i = 0; i < vectorPlayerClone.size(); ++i)
				{
					((PlayerScript*)vectorPlayerClone[i]->GetScripts()[0])->SetHealTarget(false);
				}

				if (m_type == SKILL_TYPE::SMITE || m_type == SKILL_TYPE::ZEALOUS_ACCUSATION || m_type == SKILL_TYPE::STUNNING_BLOW ||
					m_type == SKILL_TYPE::WICKED_SLICE || m_type == SKILL_TYPE::OPEN_VEIN || m_type == SKILL_TYPE::INCISION ||
					m_type == SKILL_TYPE::MACE_BASH)
				{
					for (size_t i = 0; i < vectorMonsterClone.size(); ++i)
					{
						if (((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->GetPosition() == 0 ||
							((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->GetPosition() == 1)
						{
							((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->SetBoolTarget(true);
						}
						else
						{
							((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->SetBoolTarget(false);
						}
					}
				}
				else if (m_type == SKILL_TYPE::HOLY_LANCE || m_type == SKILL_TYPE::PISTOL_SHOT || m_type == SKILL_TYPE::DISORIENTING_BLAST)
				{
					for (size_t i = 0; i < vectorMonsterClone.size(); ++i)
					{
						if (((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->GetPosition() != 0)
						{
							((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->SetBoolTarget(true);
						}
						else
						{
							((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->SetBoolTarget(false);
						}
					}
				}
				else if (m_type == SKILL_TYPE::GRAPESHOT_BLAST)
				{
					for (size_t i = 0; i < vectorMonsterClone.size(); ++i)
					{
						((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->SetBoolTarget(true);
					}
				}
				else if (m_type == SKILL_TYPE::PLAGUE_GRENADE)
				{
					for (size_t i = 0; i < vectorMonsterClone.size(); ++i)
					{
						if (((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->GetPosition() == 2 ||
							((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->GetPosition() == 3)
						{
							((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->SetBoolTarget(true);
						}
						else
						{
							((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->SetBoolTarget(false);
						}
					}
				}
				else if (m_type == SKILL_TYPE::DAZZLING_LIGHT)
				{
					for (size_t i = 0; i < vectorMonsterClone.size(); ++i)
					{
						if (((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->GetPosition() != 3)
						{
							((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->SetBoolTarget(true);
						}
						else
						{
							((MonsterScript*)vectorMonsterClone[i]->GetScripts()[0])->SetBoolTarget(false);
						}
					}
				}
			}
		}
	}
}

void SkillButtonScript::SetOwnerByName(const wstring& _playerName)
{
	GameObject* player = SceneManager::GetInst()->FindObjByName(_playerName);

	if (nullptr == player)
		return;

	const vector<Script*>& vectorScript = player->GetScripts();

	for (size_t i = 0; i < vectorScript.size(); ++i)
	{
		if ((UINT)SCRIPT_TYPE::CRUSADERSCRIPT == vectorScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::HIGHWAYMANSCRIPT == vectorScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::VESTALSCRIPT == vectorScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::PLAGUEDOCTORSCRIPT == vectorScript[i]->GetScriptType())
		{
			m_ownerScript = (PlayerScript*)vectorScript[i];
		}
	}
}

void SkillButtonScript::SetSkillButtonTexture()
{
	// 스킬별 이미지
	Ptr<Texture> skill;

	if (m_type == SKILL_TYPE::SMITE)
		skill = ResourceManager::GetInst()->Load<Texture>(L"Smite", L"texture\\crusader\\crusader.ability.one.png");
	else if (m_type == SKILL_TYPE::ZEALOUS_ACCUSATION)
		skill = ResourceManager::GetInst()->Load<Texture>(L"Zealous_Accusation", L"texture\\crusader\\crusader.ability.two.png");
	else if (m_type == SKILL_TYPE::HOLY_LANCE)
		skill = ResourceManager::GetInst()->Load<Texture>(L"Holy_Lance", L"texture\\crusader\\crusader.ability.six.png");
	else if (m_type == SKILL_TYPE::STUNNING_BLOW)
		skill = ResourceManager::GetInst()->Load<Texture>(L"Stunning_Blow", L"texture\\crusader\\crusader.ability.three.png");
	else if (m_type == SKILL_TYPE::WICKED_SLICE)
		skill = ResourceManager::GetInst()->Load<Texture>(L"Wicked_Slice", L"texture\\highwayman\\highwayman.ability.one.png");
	else if (m_type == SKILL_TYPE::PISTOL_SHOT)
		skill = ResourceManager::GetInst()->Load<Texture>(L"Pistol_Shot", L"texture\\highwayman\\highwayman.ability.two.png");
	else if (m_type == SKILL_TYPE::GRAPESHOT_BLAST)
		skill = ResourceManager::GetInst()->Load<Texture>(L"Grapeshot_Blast", L"texture\\highwayman\\highwayman.ability.four.png");
	else if (m_type == SKILL_TYPE::OPEN_VEIN)
		skill = ResourceManager::GetInst()->Load<Texture>(L"Open_Vein", L"texture\\highwayman\\highwayman.ability.seven.png");
	else if (m_type == SKILL_TYPE::PLAGUE_GRENADE)
		skill = ResourceManager::GetInst()->Load<Texture>(L"PlagueGrenade", L"texture\\plague_doctor\\plague_doctor.ability.two.png");
	else if (m_type == SKILL_TYPE::INCISION)
		skill = ResourceManager::GetInst()->Load<Texture>(L"Incision", L"texture\\plague_doctor\\plague_doctor.ability.four.png");
	else if (m_type == SKILL_TYPE::BATTLEFIELD_MEDICINE)
		skill = ResourceManager::GetInst()->Load<Texture>(L"Battlefield_Madicine", L"texture\\plague_doctor\\plague_doctor.ability.five.png");
	else if (m_type == SKILL_TYPE::DISORIENTING_BLAST)
		skill = ResourceManager::GetInst()->Load<Texture>(L"Disrienting_Blast", L"texture\\plague_doctor\\plague_doctor.ability.seven.png");
	else if (m_type == SKILL_TYPE::MACE_BASH)
		skill = ResourceManager::GetInst()->Load<Texture>(L"Mace_Bash", L"texture\\vestal\\vestal.ability.two.png");
	else if (m_type == SKILL_TYPE::DAZZLING_LIGHT)
		skill = ResourceManager::GetInst()->Load<Texture>(L"Dazzling_Light", L"texture\\vestal\\vestal.ability.three.png");
	else if (m_type == SKILL_TYPE::DIVINE_GRACE)
		skill = ResourceManager::GetInst()->Load<Texture>(L"Divine_Grace", L"texture\\vestal\\vestal.ability.four.png");
	else if (m_type == SKILL_TYPE::DIVINE_COMFORT)
		skill = ResourceManager::GetInst()->Load<Texture>(L"Divine_Comfort", L"texture\\vestal\\vestal.ability.five.png");

	Ptr<Material> material = GetMeshRender()->GetSharedMaterial();
	material->SetData(SHADER_PARAM::TEXTURE_0, skill.Get());
}

void SkillButtonScript::SetMoveTexture()
{
	// 무브 이미지
	Ptr<Texture> move = ResourceManager::GetInst()->Load<Texture>(L"Ability_Move", L"texture\\UI\\ability_move.png");
	Ptr<Material> material = GetMeshRender()->GetSharedMaterial();
	material->SetData(SHADER_PARAM::TEXTURE_0, move.Get());
}

void SkillButtonScript::SetPassTexture()
{
	// 패스 이미지
	Ptr<Texture> pass = ResourceManager::GetInst()->Load<Texture>(L"Ability_Pass", L"texture\\UI\\ability_pass.png");
	Ptr<Material> material = GetMeshRender()->GetSharedMaterial();
	material->SetData(SHADER_PARAM::TEXTURE_0, pass.Get());
}

void SkillButtonScript::SetPortrait()
{
	// 초상화 이미지
	Ptr<Texture> portrait;

	if(m_ownerName == L"Crusader")
		portrait = ResourceManager::GetInst()->Load<Texture>(L"Crusader_Portrait", L"texture\\crusader\\crusader_portrait_roster.png");
	else if (m_ownerName == L"Highwayman")
		portrait = ResourceManager::GetInst()->Load<Texture>(L"Highwayman_Portrait", L"texture\\highwayman\\highwayman_portrait_roster.png");
	else if (m_ownerName == L"PlagueDoctor")
		portrait = ResourceManager::GetInst()->Load<Texture>(L"PlagueDoctor_Portrait", L"texture\\plague_doctor\\plague_doctor_portrait_roster.png");
	else if (m_ownerName == L"Vestal")
		portrait = ResourceManager::GetInst()->Load<Texture>(L"Vestal_Portrait", L"texture\\vestal\\vestal_portrait_roster.png");

	Ptr<Material> material = GetMeshRender()->GetSharedMaterial();
	material->SetData(SHADER_PARAM::TEXTURE_0, portrait.Get());
}

void SkillButtonScript::SaveToScene(FILE* _file)
{
	SaveToFile(&m_position, _file);
	SaveToFile(&m_bPass, _file);
	SaveToFile(&m_bMove, _file);
	SaveToFile(&m_bPortrait, _file);
}

void SkillButtonScript::LoadFromScene(FILE* _file)
{
	LoadFromFile(&m_position, _file);
	LoadFromFile(&m_bPass, _file);
	LoadFromFile(&m_bMove, _file);
	LoadFromFile(&m_bPortrait, _file);
}
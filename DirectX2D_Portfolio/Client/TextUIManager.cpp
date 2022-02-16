#include "pch.h"
#include "TextUIManager.h"
#include <Engine/GameObject.h>
#include <Engine/SceneManager.h>
#include <Script/TextUIScript.h>
#include <Script/PlayerScript.h>
#include <Script/MonsterScript.h>
#include <Script/ScriptManager.h>
#include <Script/ItemManagerScript.h>

TextUIManager::TextUIManager()
{
}

TextUIManager::~TextUIManager()
{
}

void TextUIManager::init()
{
}

void TextUIManager::progress()
{
	AddTextUI();

	for (size_t i = 0; i < m_vectorTextUI.size(); ++i)
	{
		if (m_vectorTextUI[i]->GetTargetType() == TEXT_TARGET_TYPE::PLAYER || m_vectorTextUI[i]->GetTargetType() == TEXT_TARGET_TYPE::MONSTER)
		{
			if (nullptr != m_vectorTextUI[i]->GetTarget())
			{
				if (m_vectorTextUI[i]->GetTarget()->GetObjectType() == OBJECT_TYPE::PLAYER)
				{
					SetTarget(m_vectorTextUI[i]);
					PlayerTextUI(m_vectorTextUI[i]);
				}
				else if(m_vectorTextUI[i]->GetTarget()->GetObjectType() == OBJECT_TYPE::MONSTER)
					MonsterTextUI(m_vectorTextUI[i]);
			}
			else
			{
				SetTarget(m_vectorTextUI[i]);
			}
		}
		else if (m_vectorTextUI[i]->GetTargetType() == TEXT_TARGET_TYPE::ITEM)
		{
			SetTarget(m_vectorTextUI[i]);
			ItemTextUI(m_vectorTextUI[i]);
		}
	}

	m_vectorTextUI.clear();
}

void TextUIManager::AddTextUI()
{
	const vector<GameObject*>& vectorText = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::UI);

	for (size_t i = 0; i < vectorText.size(); ++i)
	{
		const vector<Script*>& vectorScript = vectorText[i]->GetScripts();

		for (size_t j = 0; j < vectorScript.size(); ++j)
		{
			if (vectorScript[j]->GetScriptType() == (UINT)SCRIPT_TYPE::TEXTUISCRIPT)
			{
				m_vectorTextUI.push_back((TextUIScript*)vectorScript[j]);
			}
		}
	}
}

void TextUIManager::SetTarget(TextUIScript* _textUI)
{
	if (_textUI->GetTargetType() == TEXT_TARGET_TYPE::PLAYER)
	{
		_textUI->SetTarget(PlayerScript::m_selectPlayer);
	}
	else if(_textUI->GetTargetType() == TEXT_TARGET_TYPE::MONSTER)
	{
		const vector<GameObject*>& vectorMonster = SceneManager::GetInst()->FindObjByType(OBJECT_TYPE::MONSTER);

		for (size_t i = 0; i < vectorMonster.size(); ++i)
		{
			const vector<Script*>& vectorMonsterScript = vectorMonster[i]->GetScripts();

			for (size_t j = 0; j < vectorMonsterScript.size(); ++j)
			{
				if ((UINT)SCRIPT_TYPE::CULTISTBRAWLERSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
					(UINT)SCRIPT_TYPE::CULTISTWITCHSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
					(UINT)SCRIPT_TYPE::MADMANSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
					(UINT)SCRIPT_TYPE::PEWSMALLSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
					(UINT)SCRIPT_TYPE::PEWMEDIUMSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
					(UINT)SCRIPT_TYPE::PEWLARGESCRIPT == vectorMonsterScript[j]->GetScriptType() ||
					(UINT)SCRIPT_TYPE::SKELETONARBALISTSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
					(UINT)SCRIPT_TYPE::SKELETONCOMMONSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
					(UINT)SCRIPT_TYPE::SKELETONCOURTIERSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
					(UINT)SCRIPT_TYPE::SKELETONDEFENDERSCRIPT == vectorMonsterScript[j]->GetScriptType())
				{
					if (((MonsterScript*)vectorMonsterScript[j])->IsInfoOpen())
					{
						_textUI->SetTarget(vectorMonster[i]);
						_textUI->SetShow(true);
					}
				}
			}
		}
	}
	else if (_textUI->GetTargetType() == TEXT_TARGET_TYPE::ITEM)
	{
		GameObject* torch = SceneManager::GetInst()->FindObjByName(L"Item_Torch");

		_textUI->SetTarget(torch);
	}
}

void TextUIManager::PlayerTextUI(TextUIScript* _textUI)
{
	const vector<Script*>& vectorPlayerScript = _textUI->GetTarget()->GetScripts();
	PlayerScript* targetScript = nullptr;

	for (size_t i = 0; i < vectorPlayerScript.size(); ++i)
	{
		if ((UINT)SCRIPT_TYPE::CRUSADERSCRIPT == vectorPlayerScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::HIGHWAYMANSCRIPT == vectorPlayerScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::VESTALSCRIPT == vectorPlayerScript[i]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::PLAGUEDOCTORSCRIPT == vectorPlayerScript[i]->GetScriptType())
		{
			targetScript = (PlayerScript*)vectorPlayerScript[i];
		}
	}

	if (_textUI->GetUsage() == TEXT_USAGE::NAME)
	{
		if ((UINT)SCRIPT_TYPE::CRUSADERSCRIPT == targetScript->GetScriptType())
		{
			_textUI->SetText(L"Raynald");
		}
		else if ((UINT)SCRIPT_TYPE::HIGHWAYMANSCRIPT == targetScript->GetScriptType())
		{
			_textUI->SetText(L"Dismas");
			Vector2 pos = _textUI->GetTextPos();
			pos.x = 305.f;
			_textUI->SetTextPos(pos);
		}
		else if ((UINT)SCRIPT_TYPE::VESTALSCRIPT == targetScript->GetScriptType())
		{
			_textUI->SetText(L"Junia");
			Vector2 pos = _textUI->GetTextPos();
			pos.x = 320.f;
			_textUI->SetTextPos(pos);
		}
		else if((UINT)SCRIPT_TYPE::PLAGUEDOCTORSCRIPT == targetScript->GetScriptType())
		{
			_textUI->SetText(L"Paracelsus");
			Vector2 pos = _textUI->GetTextPos();
			pos.x = 275.f;
			_textUI->SetTextPos(pos);
		}
	}
	else if (_textUI->GetUsage() == TEXT_USAGE::JOB)
	{
		if ((UINT)SCRIPT_TYPE::CRUSADERSCRIPT == targetScript->GetScriptType())
		{
			_textUI->SetText(L"������");
		}
		else if ((UINT)SCRIPT_TYPE::HIGHWAYMANSCRIPT == targetScript->GetScriptType())
		{
			_textUI->SetText(L"��󰭵�");
			Vector2 pos = _textUI->GetTextPos();
			pos.x = 314.f;
			_textUI->SetTextPos(pos);
		}
		else if ((UINT)SCRIPT_TYPE::VESTALSCRIPT == targetScript->GetScriptType())
		{
			_textUI->SetText(L"����");
			Vector2 pos = _textUI->GetTextPos();
			pos.x = 340.f;
			_textUI->SetTextPos(pos);
		}
		else if ((UINT)SCRIPT_TYPE::PLAGUEDOCTORSCRIPT == targetScript->GetScriptType())
		{
			_textUI->SetText(L"���� �ǻ�");
			Vector2 pos = _textUI->GetTextPos();
			pos.x = 310.f;
			_textUI->SetTextPos(pos);
		}
	}
	else if (_textUI->GetUsage() == TEXT_USAGE::HP)
	{
		wchar_t buffer[255];
		swprintf_s(buffer, L"%2d / %2d", targetScript->GetCurHP(), targetScript->GetMaxHP());
		_textUI->SetText(buffer);
	}
	else if (_textUI->GetUsage() == TEXT_USAGE::STRESS)
	{
		wchar_t buffer[255];
		swprintf_s(buffer, L"%3d / %3d", targetScript->GetCurStress(), targetScript->GetMaxStress());
		_textUI->SetText(buffer);
	}
	else if (_textUI->GetUsage() == TEXT_USAGE::ACCURACY)
	{
		wchar_t buffer[255];
		swprintf_s(buffer, L"���߷�     %d", targetScript->GetAccuracyModifier());
		_textUI->SetText(buffer);
	}
	else if (_textUI->GetUsage() == TEXT_USAGE::CRITICAL)
	{
		wchar_t buffer[255];
		swprintf_s(buffer, L"ġ��Ÿ�� %d%%", targetScript->GetCriticalChance());
		_textUI->SetText(buffer);
	}
	else if (_textUI->GetUsage() == TEXT_USAGE::DODGE)
	{
		wchar_t buffer[255];
		swprintf_s(buffer, L"ȸ����     %d", targetScript->GetDodge());
		_textUI->SetText(buffer);
	}
	else if (_textUI->GetUsage() == TEXT_USAGE::PROTECTION)
	{
		wchar_t buffer[255];
		swprintf_s(buffer, L"����     %d%%", targetScript->GetProtection());
		_textUI->SetText(buffer);
	}
	else if (_textUI->GetUsage() == TEXT_USAGE::SPEED)
	{
		wchar_t buffer[255];
		swprintf_s(buffer, L"��ø��     %d", targetScript->GetSpeed());
		_textUI->SetText(buffer);
	}
	else if (_textUI->GetUsage() == TEXT_USAGE::DAMAGE)
	{
		wchar_t buffer[255];
		swprintf_s(buffer, L"���ط�     %d-%d", targetScript->GetDamage().x, targetScript->GetDamage().y);
		_textUI->SetText(buffer);
	}
}

void TextUIManager::MonsterTextUI(TextUIScript* _textUI)
{
	const vector<Script*>& vectorMonsterScript = _textUI->GetTarget()->GetScripts();

	for (size_t j = 0; j < vectorMonsterScript.size(); ++j)
	{
		if ((UINT)SCRIPT_TYPE::CULTISTBRAWLERSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::CULTISTWITCHSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::MADMANSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::PEWSMALLSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::PEWMEDIUMSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::PEWLARGESCRIPT == vectorMonsterScript[j]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::SKELETONARBALISTSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::SKELETONCOMMONSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::SKELETONCOURTIERSCRIPT == vectorMonsterScript[j]->GetScriptType() ||
			(UINT)SCRIPT_TYPE::SKELETONDEFENDERSCRIPT == vectorMonsterScript[j]->GetScriptType())
		{
			if (!(((MonsterScript*)vectorMonsterScript[j])->IsInfoOpen()))
			{
				_textUI->SetShow(false);
				_textUI->SetTarget(nullptr);
				return;
			}
			else
			{
				if (_textUI->GetUsage() == TEXT_USAGE::NAME)
				{
					if ((UINT)SCRIPT_TYPE::CULTISTBRAWLERSCRIPT == vectorMonsterScript[j]->GetScriptType())
					{
						_textUI->SetText(L"���ŵ� �ο��");
					}
					else if ((UINT)SCRIPT_TYPE::CULTISTWITCHSCRIPT == vectorMonsterScript[j]->GetScriptType())
					{
						_textUI->SetText(L"������ ���ŵ�");
					}
					else if ((UINT)SCRIPT_TYPE::MADMANSCRIPT == vectorMonsterScript[j]->GetScriptType())
					{
						_textUI->SetText(L"����");
					}
					else if ((UINT)SCRIPT_TYPE::SKELETONARBALISTSCRIPT == vectorMonsterScript[j]->GetScriptType())
					{
						_textUI->SetText(L"�ذ� ���ú�");
					}
					else if ((UINT)SCRIPT_TYPE::SKELETONCOMMONSCRIPT == vectorMonsterScript[j]->GetScriptType())
					{
						_textUI->SetText(L"�ذ� ����");
					}
					else if ((UINT)SCRIPT_TYPE::SKELETONCOURTIERSCRIPT == vectorMonsterScript[j]->GetScriptType())
					{
						_textUI->SetText(L"�ذ� ���°�");
					}
					else if ((UINT)SCRIPT_TYPE::SKELETONDEFENDERSCRIPT == vectorMonsterScript[j]->GetScriptType())
					{
						_textUI->SetText(L"�ذ� ����");
					}
					else if ((UINT)SCRIPT_TYPE::PEWSMALLSCRIPT == vectorMonsterScript[j]->GetScriptType())
					{
						_textUI->SetText(L"�������� ����");
					}
					else if ((UINT)SCRIPT_TYPE::PEWMEDIUMSCRIPT == vectorMonsterScript[j]->GetScriptType())
					{
						_textUI->SetText(L"�ڻ쳭 ����");
					}
					else if ((UINT)SCRIPT_TYPE::PEWLARGESCRIPT == vectorMonsterScript[j]->GetScriptType())
					{
						_textUI->SetText(L"���� �渷��");
					}
				}
				else if (_textUI->GetUsage() == TEXT_USAGE::JOB)
				{
					// ���� Ÿ��
					MONSTER_TYPE type = ((MonsterScript*)vectorMonsterScript[j])->GetMonsterType();

					if (MONSTER_TYPE::HUMAN == type)
					{
						_textUI->SetText(L"�ΰ�");
					}
					else if (MONSTER_TYPE::PROFANE == type)
					{
						_textUI->SetText(L"�Ұ�");
					}
					else if (MONSTER_TYPE::WOODWORK == type)
					{
						_textUI->SetText(L"���ǰ");
					}
				}
				else if (_textUI->GetUsage() == TEXT_USAGE::HP)
				{
					wchar_t buffer[255];
					swprintf_s(buffer, L"ü��: %2d / %2d", ((MonsterScript*)vectorMonsterScript[j])->GetCurHP(), ((MonsterScript*)vectorMonsterScript[j])->GetMaxHP());
					_textUI->SetText(buffer);
				}
				else if (_textUI->GetUsage() == TEXT_USAGE::PROTECTION)
				{
					wchar_t buffer[255];
					swprintf_s(buffer, L"����: %d%%", ((MonsterScript*)vectorMonsterScript[j])->GetProtection());
					_textUI->SetText(buffer);
				}
				else if (_textUI->GetUsage() == TEXT_USAGE::DODGE)
				{
					wchar_t buffer[255];
					swprintf_s(buffer, L"ȸ��: %d", ((MonsterScript*)vectorMonsterScript[j])->GetDodge());
					_textUI->SetText(buffer);
				}
				else if (_textUI->GetUsage() == TEXT_USAGE::SPEED)
				{
					wchar_t buffer[255];
					swprintf_s(buffer, L"��ø: %d", ((MonsterScript*)vectorMonsterScript[j])->GetSpeed());
					_textUI->SetText(buffer);
				}
				else if (_textUI->GetUsage() == TEXT_USAGE::RESISTANCE_STUN)
				{
					wchar_t buffer[255];
					swprintf_s(buffer, L"����\t%d%%", ((MonsterScript*)vectorMonsterScript[j])->GetStunResistance());
					_textUI->SetText(buffer);
				}
				else if (_textUI->GetUsage() == TEXT_USAGE::RESISTANCE_BLIGHT)
				{
					wchar_t buffer[255];
					swprintf_s(buffer, L"�ߵ�\t%d%%", ((MonsterScript*)vectorMonsterScript[j])->GetBlightResistance());
					_textUI->SetText(buffer);
				}
				else if (_textUI->GetUsage() == TEXT_USAGE::RESISTANCE_BLEED)
				{
					wchar_t buffer[255];
					swprintf_s(buffer, L"����\t%d%%", ((MonsterScript*)vectorMonsterScript[j])->GetBleedResistance());
					_textUI->SetText(buffer);
				}
				else if (_textUI->GetUsage() == TEXT_USAGE::RESISTANCE_DEBUFF)
				{
					wchar_t buffer[255];
					swprintf_s(buffer, L"�����\t%d%%", ((MonsterScript*)vectorMonsterScript[j])->GetDebuffResistance());
					_textUI->SetText(buffer);
				}
				else if (_textUI->GetUsage() == TEXT_USAGE::RESISTANCE_MOVE)
				{
					wchar_t buffer[255];
					swprintf_s(buffer, L"�̵�\t%d%%", ((MonsterScript*)vectorMonsterScript[j])->GetMoveResistance());
					_textUI->SetText(buffer);
				}
			}
		}
		
	}
}

void TextUIManager::ItemTextUI(TextUIScript* _textUI)
{
	const vector<Script*>& vectorScript = _textUI->GetTarget()->GetScripts();
	GameObject* itemManager = SceneManager::GetInst()->FindObjByName(L"ItemManager");
	const vector<Script*>& vectorItemManagerScript = itemManager->GetScripts();
	ITEM_TYPE type = ITEM_TYPE::TORCH;
	int count = 0;

	for (size_t i = 0; i < vectorScript.size(); ++i)
	{
		if (vectorScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::ITEMSCRIPT)
		{
			type = ((ItemScript*)vectorScript[i])->GetItemType();
		}
	}

	for (size_t i = 0; i < vectorItemManagerScript.size(); ++i)
	{
		if (vectorItemManagerScript[i]->GetScriptType() == (UINT)SCRIPT_TYPE::ITEMMANAGERSCRIPT)
		{
			count = ((ItemManagerScript*)vectorItemManagerScript[i])->GetItemCount(type);
		}
	}

	if (_textUI->GetUsage() == TEXT_USAGE::ITEM_COUNT)
	{
		if (type == ITEM_TYPE::TORCH)
		{
			wchar_t buffer[255];
			swprintf_s(buffer, L"%2d", count);
			_textUI->SetText(buffer);
		}
	}
}

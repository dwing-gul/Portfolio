#include "pch.h"
#include "HPBarScript.h"
#include "PlayerScript.h"
#include "MonsterScript.h"
#include <Engine/GameObject.h>
#include <Engine/Transform.h>
#include <Engine/MeshRender.h>
#include <Engine/Animator2D.h>
#include <Engine/Animation2D.h>
#include <Engine/TimeManager.h>

HPBarScript::HPBarScript() :
	UIScript(SCRIPT_TYPE::HPBARSCRIPT),
	m_owner(nullptr),
	m_bBase(false),
	m_bStress(false),
	m_bHit(false),
	m_bHeal(false),
	m_bFirst(true),
	m_length(0.f),
	m_resultLength(0.f),
	m_stress(0)
{
}

HPBarScript::~HPBarScript()
{
}

void HPBarScript::start()
{
	UIScript::start();

	m_sharedMaterial = GetMeshRender()->GetSharedMaterial();
	m_cloneMaterial = GetMeshRender()->GetCloneMaterial();
	GetMeshRender()->SetMaterial(m_cloneMaterial);

	m_owner = Object()->GetParent();

	// 위치 조정
	if (!m_owner)
		return;

	m_length = GetTransform()->GetLocalScale().x / m_owner->GetTransform()->GetLocalScale().x;

	if (m_bStress && !m_bBase)
	{
		Object()->AddComponent(new Animator2D);
		GetAnimator2D()->LoadAnim(L"anim\\stress_bar\\StressBar.anim");
		GetAnimator2D()->Play(L"StressBar");
		Animation2D* anim = GetAnimator2D()->FindAnim(L"StressBar");
		vector<tAnimFrame>& vectorFrame = anim->GetVectorAnimFrame();
		vectorFrame[0].frameSize.x = 0.f;
	}
}

void HPBarScript::update()
{
	UIScript::update();

	if (m_bFirst)
	{
		Vector3 parentPos = m_owner->GetTransform()->GetLocalPos();

		SetOriginXPos(parentPos.x);

		if (m_bStress)
		{
			SetOriginYPos(parentPos.y - 160.f);
			if (!m_bBase)
				SetOriginXPos(-40.f);
		}
		else
			SetOriginYPos(parentPos.y - 150.f);

		m_bFirst = false;
	}
}

void HPBarScript::lateUpdate()
{
	UIScript::lateUpdate();

	if (!m_owner)
		return;

	if (!m_bBase)
	{
		if (m_bStress) // 스트레스 바일 때 동작
		{
			if (m_owner->GetObjectType() == OBJECT_TYPE::PLAYER)
			{
				Animation2D* anim = GetAnimator2D()->FindAnim(L"StressBar");
				vector<tAnimFrame>& vectorFrame = anim->GetVectorAnimFrame();

				const vector<Script*>& vectorPlayerScript = m_owner->GetScripts();
				for (size_t i = 0; i < vectorPlayerScript.size(); ++i)
				{
					if ((UINT)SCRIPT_TYPE::CRUSADERSCRIPT == vectorPlayerScript[i]->GetScriptType() ||
						(UINT)SCRIPT_TYPE::HIGHWAYMANSCRIPT == vectorPlayerScript[i]->GetScriptType() ||
						(UINT)SCRIPT_TYPE::VESTALSCRIPT == vectorPlayerScript[i]->GetScriptType() ||
						(UINT)SCRIPT_TYPE::PLAGUEDOCTORSCRIPT == vectorPlayerScript[i]->GetScriptType())
					{
						float ratio = (float)((PlayerScript*)vectorPlayerScript[i])->GetCurStress() / ((PlayerScript*)vectorPlayerScript[i])->GetMaxStress();

						float scale = vectorFrame[0].frameSize.x;
						Vector3 pos = GetTransform()->GetLocalPos();

						if (((PlayerScript*)vectorPlayerScript[i])->GetCurStress() != m_stress)
						{
							m_stress = ((PlayerScript*)vectorPlayerScript[i])->GetCurStress();
							m_resultLength = ratio;
							m_bHit = true;
						}

						if (m_bHit)
						{
							scale += 0.1f * DELTATIME;

							if (scale > m_resultLength)
							{
								scale = m_resultLength;
								m_bHit = false;
							}
							else
							{
								pos.x += 0.0057f * DELTATIME;
								vectorFrame[0].frameSize.x = scale;
								GetTransform()->SetLocalPos(pos);
							}
						}
					}
				}
			}
		}
		else // 일반 hp 바일 때의 동작
		{
			if (m_owner->GetObjectType() == OBJECT_TYPE::PLAYER)
			{
				const vector<Script*>& vectorPlayerScript = m_owner->GetScripts();
				for (size_t i = 0; i < vectorPlayerScript.size(); ++i)
				{
					if ((UINT)SCRIPT_TYPE::CRUSADERSCRIPT == vectorPlayerScript[i]->GetScriptType() ||
						(UINT)SCRIPT_TYPE::HIGHWAYMANSCRIPT == vectorPlayerScript[i]->GetScriptType() ||
						(UINT)SCRIPT_TYPE::VESTALSCRIPT == vectorPlayerScript[i]->GetScriptType() ||
						(UINT)SCRIPT_TYPE::PLAGUEDOCTORSCRIPT == vectorPlayerScript[i]->GetScriptType())
					{
						float ratio = (float)((PlayerScript*)vectorPlayerScript[i])->GetCurHP() / ((PlayerScript*)vectorPlayerScript[i])->GetMaxHP();

						Vector3 scale = GetTransform()->GetLocalScale();
						Vector3 pos = GetTransform()->GetLocalPos();

						if (((PlayerScript*)vectorPlayerScript[i])->IsHit())
						{
							m_resultLength = m_length * ratio;

							if (((PlayerScript*)vectorPlayerScript[i])->GetMaxHP() * ratio < 0.f)
								m_resultLength = 0.f;

							m_bHit = true;
							((PlayerScript*)vectorPlayerScript[i])->SetHit(false);
						}
						else if(((PlayerScript*)vectorPlayerScript[i])->IsHeal())
						{
							m_resultLength = m_length * ratio;

							if (((PlayerScript*)vectorPlayerScript[i])->GetMaxHP() * ratio > m_length)
								m_resultLength = m_length;

							m_bHeal = true;
							((PlayerScript*)vectorPlayerScript[i])->SetHeal(false);
						}

						if (m_bHit)
						{
							scale.x -= 0.01f * DELTATIME;

							if (scale.x < m_resultLength)
							{
								scale.x = m_resultLength;
								m_bHit = false;
							}
							else
							{
								pos.x -= 0.005f * DELTATIME;
								GetTransform()->SetLocalScale(scale);
								GetTransform()->SetLocalPos(pos);
							}
						}
						
						if (m_bHeal)
						{
							scale.x += 0.01f * DELTATIME;

							if (scale.x > m_resultLength)
							{
								scale.x = m_resultLength;
							}
							else
							{
								pos.x += 0.005f * DELTATIME;
								GetTransform()->SetLocalScale(scale);
								GetTransform()->SetLocalPos(pos);
							}
						}
					}
				}
			}
			else if (m_owner->GetObjectType() == OBJECT_TYPE::MONSTER)
			{
				const vector<Script*>& vectorMonsterScript = m_owner->GetScripts();
				for (size_t i = 0; i < vectorMonsterScript.size(); ++i)
				{
					if ((UINT)SCRIPT_TYPE::CULTISTBRAWLERSCRIPT == vectorMonsterScript[i]->GetScriptType() ||
						(UINT)SCRIPT_TYPE::CULTISTWITCHSCRIPT == vectorMonsterScript[i]->GetScriptType() ||
						(UINT)SCRIPT_TYPE::MADMANSCRIPT == vectorMonsterScript[i]->GetScriptType() ||
						(UINT)SCRIPT_TYPE::PEWSMALLSCRIPT == vectorMonsterScript[i]->GetScriptType() ||
						(UINT)SCRIPT_TYPE::PEWMEDIUMSCRIPT == vectorMonsterScript[i]->GetScriptType() ||
						(UINT)SCRIPT_TYPE::PEWLARGESCRIPT == vectorMonsterScript[i]->GetScriptType() ||
						(UINT)SCRIPT_TYPE::SKELETONARBALISTSCRIPT == vectorMonsterScript[i]->GetScriptType() ||
						(UINT)SCRIPT_TYPE::SKELETONCOMMONSCRIPT == vectorMonsterScript[i]->GetScriptType() ||
						(UINT)SCRIPT_TYPE::SKELETONCOURTIERSCRIPT == vectorMonsterScript[i]->GetScriptType() ||
						(UINT)SCRIPT_TYPE::SKELETONDEFENDERSCRIPT == vectorMonsterScript[i]->GetScriptType())
					{
						float ratio = (float)((MonsterScript*)vectorMonsterScript[i])->GetCurHP() / ((MonsterScript*)vectorMonsterScript[i])->GetMaxHP();

						Vector3 scale = GetTransform()->GetLocalScale();
						Vector3 pos = GetTransform()->GetLocalPos();

						if (((MonsterScript*)vectorMonsterScript[i])->IsHit())
						{
							m_resultLength = m_length * ratio;

							if (((MonsterScript*)vectorMonsterScript[i])->GetMaxHP() * ratio < 0.f)
								m_resultLength = 0.f;

							m_bHit = true;
							((MonsterScript*)vectorMonsterScript[i])->SetHit(false);
						}

						if (m_bHit)
						{
							scale.x -= 0.01f * DELTATIME;

							if (scale.x < m_resultLength)
							{
								scale.x = m_resultLength;
								m_bHit = false;

								GetTransform()->SetLocalScale(scale);
							}
							else
							{
								pos.x -= 0.005f * DELTATIME;
								GetTransform()->SetLocalScale(scale);
								GetTransform()->SetLocalPos(pos);
							}
						}
					}
				}
			}
		}
	}
}

void HPBarScript::SaveToScene(FILE* _file)
{
	SaveToFile(&m_bBase, _file);
	SaveToFile(&m_bStress, _file);
}

void HPBarScript::LoadFromScene(FILE* _file)
{
	LoadFromFile(&m_bBase, _file);
	LoadFromFile(&m_bStress, _file);
}

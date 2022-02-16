#include "pch.h"
#include "EffectScript.h"
#include "PlayerScript.h"
#include "MonsterScript.h"
#include <Engine/Animator2D.h>
#include <Engine/GameObject.h>
#include <Engine/Transform.h>

EffectScript::EffectScript() :
	Script((UINT)SCRIPT_TYPE::EFFECTSCRIPT),
	m_owner(nullptr),
	m_bChange(true),
	m_originXPos(0.f),
	m_originYPos(0.f)
{
}

EffectScript::~EffectScript()
{
}

void EffectScript::start()
{
	GetAnimator2D()->LoadAnim(L"anim\\effect\\SelectPlayerEffect.anim");
	GetAnimator2D()->LoadAnim(L"anim\\effect\\SelectTargetEffect.anim");
	GetAnimator2D()->LoadAnim(L"anim\\effect\\SelectHealTargetEffect.anim");
	GetAnimator2D()->LoadAnim(L"anim\\effect\\Target.anim");
	GetAnimator2D()->LoadAnim(L"anim\\effect\\Empty.anim");

	m_owner = Object()->GetParent();
	Vector3 pos = GetTransform()->GetLocalPos();
	Vector3 parentPos = m_owner->GetTransform()->GetLocalPos();

	m_originXPos = pos.x;
	m_originYPos = parentPos.y - 190.f;

	if (m_owner->GetObjectType() == OBJECT_TYPE::MONSTER)
	{
		m_originYPos = parentPos.y - 130.f;
	}

	Vector3 scale = GetTransform()->GetLocalScale();
	scale *= 0.6f;
	GetTransform()->SetLocalScale(scale);
}

void EffectScript::update()
{
	if (m_bChange)
	{
		m_bChange = false;
		float flag = 1.f;
		// 부모를 따라다닐 수 있도록
		Vector3 scale = GetTransform()->GetLocalScale();
		Vector3 parentScale = Object()->GetParent()->GetTransform()->GetLocalScale();
		Vector3 parentPos = Object()->GetParent()->GetTransform()->GetLocalPos();

		scale.x = scale.x / parentScale.x;
		scale.y = scale.y / parentScale.y;
		GetTransform()->SetLocalScale(scale);

		Vector3 finalPos = GetTransform()->GetLocalPos();
		finalPos.x = m_originXPos / parentScale.x;

		if (m_originXPos == parentPos.x)
			finalPos.x = 0.f;

		if (m_originYPos < parentPos.y)
			flag *= -1.f;
		else
			flag *= 1.f;

		finalPos.y = flag * (abs(m_originYPos - parentPos.y) / parentScale.y);
		finalPos.z = 20.f;

		GetTransform()->SetLocalPos(finalPos);
	}
}

void EffectScript::lateUpdate()
{
	if (m_owner->GetObjectType() == OBJECT_TYPE::MONSTER)
	{
		if (((MonsterScript*)m_owner->GetScripts()[0])->IsTurnStart())
		{
			GetAnimator2D()->Play(L"SelectTargetEffect");
		}
		else
		{
			if (((MonsterScript*)m_owner->GetScripts()[0])->IsTarget())
			{
				GetAnimator2D()->Play(L"Target");
			}
			else
			{
				GetAnimator2D()->Play(L"Empty");
			}
		}
	}

	if (m_owner->GetObjectType() == OBJECT_TYPE::PLAYER)
	{
		if (((PlayerScript*)m_owner->GetScripts()[0])->IsBattle())
		{
			if (((PlayerScript*)m_owner->GetScripts()[0])->IsTurnStart())
			{
				if (((PlayerScript*)m_owner->GetScripts()[0])->IsHealTarget())
				{
					GetAnimator2D()->Play(L"SelectHealTargetEffect");
				}
				else
				{
					GetAnimator2D()->Play(L"SelectPlayerEffect");
				}
			}
			else
			{
				if (((PlayerScript*)m_owner->GetScripts()[0])->IsHealTarget())
				{
					GetAnimator2D()->Play(L"SelectHealTargetEffect");
				}
				else
				{
					GetAnimator2D()->Play(L"Empty");
				}
			}
		}
		else
		{
			if (PlayerScript::m_selectPlayer == m_owner)
			{
				GetAnimator2D()->Play(L"SelectPlayerEffect");
			}
			else
			{
				GetAnimator2D()->Play(L"Empty");
			}
		}
	}
}

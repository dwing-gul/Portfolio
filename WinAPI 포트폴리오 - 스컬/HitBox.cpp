#include "stdafx.h"
#include "HitBox.h"
#include "Collider.h"
#include "Boss.h"
#include "Player.h"
#include "Tile.h"
#include "Effect.h"
#include "Animator.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"

HitBox::HitBox() :
	m_owner(nullptr),
	m_damage(0),
	m_offset(Vector2(0.f, 0.f)),
	m_effectOffset(Vector2(0.f, 0.f))
{
	CreateCollider();
}

HitBox::~HitBox()
{
}

void HitBox::update()
{
	Vector2 offset = m_owner->GetPos() + m_offset;
	GetCollider()->SetOffsetPos(offset);
	GetCollider()->SetScale(GetScale());
}

void HitBox::render(HDC _dc)
{
	Obj::render(_dc);
}

void HitBox::OnColliderEnter(Obj* _other)
{
	if (_other->GetName() == L"BossHead")
	{
		int a = 0;
	}

	if (m_owner->GetType() == GROUP_TYPE::PLAYER && _other->GetType() == GROUP_TYPE::MONSTER)
	{
		Scene* curScene = SceneManager::GetInst()->GetCurScene();
		Effect* effect = new Effect;
		effect->CreateAnimator();
		effect->GetAnimator()->Load(L"Data\\Anim\\Hit_Effect.anim");
		effect->GetAnimator()->PlayAnim(L"Idle", false);
		effect->SetMagnification(Vector2(0.4f, 0.4f));
		effect->SetPos(Vector2(m_owner->GetPos() + m_effectOffset));
		curScene->AddObj(effect, GROUP_TYPE::EFFECT);

		((Monster*)_other)->Hit(m_damage);
	}

	if (m_owner->GetType() == GROUP_TYPE::PLAYER && _other->GetType() == GROUP_TYPE::BOSS_BODY)
	{
		((Monster*)_other)->Hit(m_damage);
	}

	if (m_owner->GetType() == GROUP_TYPE::MONSTER && _other->GetType() == GROUP_TYPE::PLAYER)
	{
		((Player*)_other)->Hit(((Monster*)m_owner)->GetDamage());
	}
}

void HitBox::OnCollider(Obj* _other)
{
	if (((Monster*)m_owner)->GetMonsterType() == MONSTER_GROUP::BOSS && _other->GetType() == GROUP_TYPE::TILE)
	{
		Vector2 otherPos = _other->GetPos();
		Vector2 pos = GetPos();
		Vector2 ownerPos = m_owner->GetPos();
		Vector2 tileColliderPos = _other->GetCollider()->GetFinalPos();
		Vector2 dir = otherPos - pos;
		Vector2 tileColliderSize = _other->GetCollider()->GetScale() /2.f;
		if (dir != Vector2(0.f, 0.f))
			dir.normalize();

		TILE_TYPE tileType = ((Tile*)_other)->GetTileType();

		if (TILE_TYPE::GROUND == tileType)
		{
			// 타일의 위쪽에서 부딪혔을 때
			if (1.f >= dir.y)
			{
				Camera::GetInst()->AddCamEffect(CAM_EFFECT::VIBRATION, 0.05f);
				Camera::GetInst()->SetVibration(Vector2(sqrtf(2.f), sqrtf(2.f)), 0.05f);
				ownerPos.y = tileColliderPos.y - tileColliderSize.y;
				m_owner->SetPos(ownerPos);
				((Boss*)m_owner)->SetGround(true);
			}
		}
	}
}
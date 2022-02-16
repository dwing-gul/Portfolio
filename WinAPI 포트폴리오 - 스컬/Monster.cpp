#include "stdafx.h"
#include "Monster.h"
#include "AI.h"
#include "Collider.h"
#include "Texture.h"
#include "Animator.h"
#include "Animation.h"
#include "Camera.h"
#include "SceneManager.h"
#include "Scene.h"
#include "HitBox.h"

Monster::Monster() :
	m_maxHP(100),
	m_curHP(m_maxHP),
	m_damage(10),
	m_speed(100.f),
	m_range(100.f),
	m_dir(-1.f),
	m_HPRatio(1.f),
	m_monsterType(MONSTER_GROUP::FOOTMAN),
	m_hitBox(nullptr),
	m_bHit(false),
	m_bDie(false),
	m_bFirst(true),
	m_ai(nullptr)
{
}

Monster::~Monster()
{
	if (nullptr != m_ai)
		delete m_ai;

	if (nullptr != m_hitBox)
		delete m_hitBox;
}

void Monster::update()
{
	m_HPRatio = float(m_curHP) / float(m_maxHP);

	if (nullptr != m_ai && !m_bDie)
		m_ai->update();

	if (m_bDie && nullptr != m_hitBox)
	{
		DeleteObject(m_hitBox);
		m_hitBox = nullptr;
	}

	RigidBody::update();
}

void Monster::lateUpdate()
{
	if (nullptr != m_ai && !m_bDie)
		m_ai->lateUpdate();

	if (m_bDie && MONSTER_GROUP::BOSS != m_monsterType)
	{
		SetAnimDir(m_dir, L"Dead", false);

		if (GetAnimator()->GetCurAnim()->IsFinish() && !IsDead())
		{
			DeleteObject(this);
		}
	}

	if (!m_bFirst)
	{
		RigidBody::lateUpdate();
	}

	m_bFirst = false;
}

void Monster::render(HDC _dc)
{
	Scene* curScene = SceneManager::GetInst()->GetCurScene();
	if (curScene->GetType() == SCENE_TYPE::TOOL)
	{
		Vector2 renderPos = Camera::GetInst()->GetRenderPos(GetPos());

		if (nullptr != GetTexture())
		{
			UINT width = GetTexture()->GetWidth();
			UINT height = GetTexture()->GetHeight();

			TransparentBlt(_dc, (int)(renderPos.x - width / 2), (int)(renderPos.y - height), width, height,
				GetTexture()->GetTextureDC(), 0, 0, width, height, RGB(255, 0, 255));
		}
	}
	if (nullptr != GetAnimator())
		GetAnimator()->render(_dc);

	Obj::render(_dc);
}

void Monster::SetAnimDir(float _dir, wstring _state, float _repeat)
{
	if (_dir == 1.f)
	{
		_state += L"_Right";
		GetAnimator()->PlayAnim(_state, _repeat);
	}
	else
	{
		_state += L"_Left";
		GetAnimator()->PlayAnim(_state, _repeat);
	}
}

void Monster::CreateAI()
{
	m_ai = new AI;
	m_ai->m_monster = this;
}

void Monster::Hit(int _damage)
{
	m_bHit = true;

	m_curHP -= _damage;

	if (m_curHP <= 0)
	{
		m_bDie = true;

		m_curHP = 0;
	}
}

void Monster::CreateHitBox(Vector2 _offset, Vector2 _scale)
{
	if (m_hitBox == nullptr)
	{
		m_hitBox = new HitBox;
		m_hitBox->m_owner = this;
		m_hitBox->SetOffset(_offset);
		m_hitBox->SetScale(_scale);
		CreateObject(m_hitBox, GROUP_TYPE::MONSTER_HITBOX);
	}
}

void Monster::Save(FILE* _file)
{
	SaveWString(GetName(), _file);
	Vector2 pos = GetPos();
	fwrite(&pos, sizeof(Vector2), 2, _file);
	bool hasCollider = false;
	if (nullptr != GetCollider())
	{
		hasCollider = true;
		fwrite(&hasCollider, sizeof(bool), 1, _file);
		Vector2 offsetPos = GetCollider()->GetFinalPos() - GetPos();
		Vector2 scale = GetCollider()->GetScale();
		fwrite(&offsetPos, sizeof(Vector2), 2, _file);
		fwrite(&scale, sizeof(Vector2), 2, _file);
	}
	else
	{
		fwrite(&hasCollider, sizeof(bool), 1, _file);
	}
}

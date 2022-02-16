#include "pch.h"
#include "CBulletScript.h"
#include "CMonsterScript.h"
#include "CPlayerScript.h"
#include <Engine/CCollider3D.h>

CBulletScript::CBulletScript() :
	CScript((UINT)SCRIPT_TYPE::BULLETSCRIPT),
	m_owner(nullptr),
	m_target(nullptr),
	m_damage(0.f),
	m_critical(0.f),
	m_extraPercent(0.f),
	m_extraFlat(0.f),
	m_AttackType(ATTACK_TYPE::BASE),
	m_dist(0.f),
	m_speed(0.f),
	m_prevTargetPos(Vec3(0.f, 0.f, 0.f)),
	m_dir(Vec3(0.f, 0.f, 0.f)),
	m_bRot(true),
	m_rotAngle(0.f),
	m_rotTime(0.f),
	m_lifeTime(0.f)
{
}

CBulletScript::CBulletScript(const CBulletScript& _origin) :
	CScript(_origin),
	m_owner(nullptr),
	m_target(nullptr),
	m_damage(0.f),
	m_critical(0.f),
	m_extraPercent(0.f),
	m_extraFlat(0.f),
	m_AttackType(ATTACK_TYPE::BASE),
	m_dist(0.f),
	m_speed(0.f),
	m_prevTargetPos(Vec3(0.f, 0.f, 0.f)),
	m_dir(Vec3(0.f, 0.f, 0.f)),
	m_bRot(true),
	m_rotAngle(0.f),
	m_rotTime(0.f),
	m_lifeTime(0.f)
{
}

CBulletScript::~CBulletScript()
{
}

void CBulletScript::start()
{
	m_HitSound = CResourceManager::GetInst()->Load<CSound>(L"Bullet_Hit", L"sound\\aya\\Alex_Normal_Pistol_Hit.wav");
}

void CBulletScript::update()
{
	if (Object()->IsDead())
		return;

	Vec3 pos = Transform()->GetLocalPos();
	m_lifeTime += DELTATIME;

	if (nullptr == m_target)
	{
		m_dist += m_speed * DELTATIME;

		if (m_dist > 800.f || m_lifeTime > 5.f)
		{
			CScript::DeleteObject(Object());
		}

		pos += m_speed * m_dir * DELTATIME;

		Transform()->SetLocalPos(pos);
	}
	else
	{
		// Å¸±ê ÃßÀû
		Vec3 pointPos = m_target->Transform()->GetLocalPos();
		Vec3 dir = Vec3(0.f, 0.f, 0.f);
		Vec3 rot = Transform()->GetLocalRot();
		Vec3 front = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		float distance = 0.f;

		if (pointPos != m_prevTargetPos)
			m_bRot = true;

		dir.x = pointPos.x - pos.x;
		dir.z = pointPos.z - pos.z;
		distance = dir.Length();
		dir.Normalize();

		if (m_bRot)
		{
			m_rotAngle = 0.f;
			m_rotTime = 0.f;

			Vec3 up = Transform()->GetWorldDir(DIR_TYPE::UP);
			m_bRot = false;
			Vec3 crossVec = front.Cross(dir);
			float sign = crossVec.Dot(up);

			if (sign > 0.f)
			{
				m_rotAngle += acosf(front.Dot(dir)) - XM_PI;
			}
			else if (sign < 0.f)
			{
				m_rotAngle -= acosf(front.Dot(dir)) - XM_PI;
			}
		}

		pos += dir * m_speed * DELTATIME * 1.5f;

		float yAngle = rot.y;

		m_rotTime += DELTATIME;

		if (m_rotTime < 0.25f)
			rot.y += m_rotAngle * 4.f * DELTATIME;

		if (_isnanf(rot.y))
			rot.y = yAngle;

		Transform()->SetLocalPos(pos);
		Transform()->SetLocalRot(rot);

		m_prevTargetPos = pointPos;
	}
}

void CBulletScript::OnColliderEnter(CCollider3D* _other)
{
	if (Object()->IsDead())
		return;

	CGameObject* target = _other->Object();

	if (target == m_owner || (target->GetObjectType() != OBJECT_TYPE::PLAYER &&
		target->GetObjectType() != OBJECT_TYPE::MONSTER))
		return;

	if (((CObjectScript*)target->GetScripts()[0])->IsCharacterDead())
		return;


	if (OBJECT_TYPE::MONSTER == target->GetObjectType())
	{
		((CMonsterScript*)target->GetScripts()[0])->Hit(m_AttackType, m_damage, m_critical, m_extraPercent, m_extraFlat, m_owner);
	}
	else if (OBJECT_TYPE::PLAYER == target->GetObjectType())
	{
		((CPlayerScript*)target->GetScripts()[0])->Hit(m_AttackType, m_damage, m_critical, m_extraPercent, m_extraFlat, m_owner);
	}

	if (nullptr != m_HitSound.Get())
		m_HitSound->Play(1, 0.1f);

	CScript::DeleteObject(Object());
}


void CBulletScript::SaveToScene(FILE* _file)
{
}

void CBulletScript::LoadFromScene(FILE* _file)
{
}

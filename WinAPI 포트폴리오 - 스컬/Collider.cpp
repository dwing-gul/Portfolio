#include "stdafx.h"
#include "Collider.h"
#include "Obj.h"
#include "Core.h"
#include "Camera.h"
#include "Boss.h"
#include "Projectile.h"
#include "KeyManager.h"

UINT Collider::g_ID = 0;
bool Collider::g_bLook = false;

Collider::Collider() :
	m_owner(nullptr),
	m_ID(g_ID++),
	m_collisionCount(0)
{
}

Collider::Collider(const Collider& _other) :
	m_owner(nullptr),
	m_offsetPos(_other.m_offsetPos),
	m_scale(_other.m_scale),
	m_ID(g_ID++),
	m_collisionCount(0)
{
}

Collider::~Collider()
{
}

void Collider::update()
{
}

void Collider::lateUpdate()
{
	if (((Monster*)m_owner)->GetMonsterType() == MONSTER_GROUP::BOSS)
	{
		Vector2 objPos = ((Boss*)m_owner)->GetFinalPos();

		m_finalPos = objPos + m_offsetPos;
	}
	else if (m_owner->GetType() == GROUP_TYPE::MONSTER_PROJECTILE)
	{
		Vector2 objPos = ((Projectile*)m_owner)->GetFinalPos();

		m_finalPos = objPos + m_offsetPos;
	}
	else
	{
		Vector2 objPos = m_owner->GetPos();

		m_finalPos = objPos + m_offsetPos;
	}
}

void Collider::render(HDC _dc)
{
	if (g_bLook)
	{
		HPEN pen = Core::GetInst()->GetPen(PEN_TYPE::BLUE);;
		Vector2 renderPos = Camera::GetInst()->GetRenderPos(m_finalPos);

		if (0 == m_collisionCount)
		{
			pen = Core::GetInst()->GetPen(PEN_TYPE::GREEN);
		}
		else if (1 == m_collisionCount)
		{
			pen = Core::GetInst()->GetPen(PEN_TYPE::RED);
		}

		HBRUSH brush = Core::GetInst()->GetBrush(BRUSH_TYPE::HOLLOW);

		HPEN prevPen = (HPEN)SelectObject(_dc, pen);
		HBRUSH prevBrush = (HBRUSH)SelectObject(_dc, brush);

		Rectangle(_dc, int(renderPos.x - (m_scale.x / 2.f)), int(renderPos.y - (m_scale.y / 2.f)),
			int(renderPos.x + (m_scale.x / 2.f)), int(renderPos.y + (m_scale.y / 2.f)));

		SelectObject(_dc, prevPen);
		SelectObject(_dc, prevBrush);
	}
}

void Collider::OnColliderEnter(Collider* _other)
{
	++m_collisionCount;
	m_owner->OnColliderEnter(_other->m_owner);
}

void Collider::OnCollider(Collider* _other)
{
	m_owner->OnCollider(_other->m_owner);
}

void Collider::OnColliderExit(Collider* _other)
{
	--m_collisionCount;

	//assert(0 <= m_collisionCount);
	m_owner->OnColliderExit(_other->m_owner);
}

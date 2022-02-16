#include "stdafx.h"
#include "Projectile.h"
#include "Animator.h"
#include "Player.h"
#include "Boss.h"
#include "Collider.h"
#include "Tile.h"
#include "Texture.h"
#include "Camera.h"
#include "TimeManager.h"

Projectile::Projectile() :
	m_owner(nullptr),
	m_projectileType(PROJECTILE_TYPE::PROJECTILE),
	m_attackType(ATTACK_TYPE::PHYSICAL),
	m_bFirstShot(false),
	m_bGround(false),
	m_bLeftWall(false),
	m_bRightWall(false),
	m_bHead(false),
	m_bDie(false),
	m_damage(0)
{
}

Projectile::~Projectile()
{
}

void Projectile::update()
{
	if (!IsDead())
	{
		if (m_projectileType == PROJECTILE_TYPE::HEAD)
		{
			int dir = ((Player*)m_owner)->GetDir();
			if (m_bFirstShot)
			{
				SetVelocity(Vector2(dir * 400.f, 0.f));
				SetSubSpeed(180.f);
				m_bFirstShot = false;
			}

			if (GetVelocity() == Vector2(0.f, 0.f) && !m_bGround)
			{
				AddVelocity(Vector2(0.f, 150.f));
				SetFriction(false);
			}

		}

		if (((Monster*)m_owner)->GetMonsterType() == MONSTER_GROUP::BOSS)
		{
			Vector2 objPos = ((Boss*)m_owner)->GetFinalPos();

			m_finalPos = objPos + m_offsetPos;
		}

		RigidBody::update();
	}
}

void Projectile::lateUpdate()
{
	if (!IsDead())
	{
		if (m_projectileType == PROJECTILE_TYPE::BOSS_SMALL_PROJECTILE)
		{
			Vector2 pos = m_offsetPos;
			m_dir.normalize();

			pos += m_dir * 200.f * DELTATIME;

			m_offsetPos = pos;
		}
		else if (m_projectileType == PROJECTILE_TYPE::BOSS_PROJECTILE)
		{
			Vector2 pos = m_finalPos;
			Vector2 dir = pos - m_dir;
			dir.normalize();

			if (dir.y > 0)
			{
				dir.y = -dir.y;
			}

			m_offsetPos -= dir * 200.f * DELTATIME;
		}

		if (m_bDie)
		{
			DeleteObject(this);
		}

		RigidBody::lateUpdate();
	}
}

void Projectile::render(HDC _dc)
{
	Texture* tex = GetTexture();
	if (nullptr != tex)
	{
		Vector2 pos = Camera::GetInst()->GetRenderPos(m_finalPos);
		UINT width = tex->GetWidth();
		UINT height = tex->GetHeight();

		BLENDFUNCTION blend = {};
		blend.AlphaFormat = AC_SRC_ALPHA;
		blend.BlendFlags = 0;
		blend.BlendOp = AC_SRC_OVER;
		blend.SourceConstantAlpha = 255;

		AlphaBlend(_dc, (int)pos.x - width / 2, (int)pos.y - height / 2, int((float)width * 1.5f), int((float)height * 1.5f),
			tex->GetTextureDC(), 0, 0, width, height, blend);
	}
	if (nullptr != GetAnimator())
	{
		GetAnimator()->render(_dc);
	}

	Obj::render(_dc);
}

void Projectile::OnColliderEnter(Obj* _other)
{
	if (m_owner->GetType() == GROUP_TYPE::PLAYER && _other->GetType() == GROUP_TYPE::MONSTER)
	{
		if (PROJECTILE_TYPE::HEAD == m_projectileType)
		{
			if (!m_bHead)
			{
				int dir = ((Player*)m_owner)->GetDir();
				int damage = m_damage;
				
				/*{
				  아이템 효과에 따른 데미지 증감처리를 여기서 계산
				}*/

				((Monster*)_other)->Hit(damage);
				SetVelocity(Vector2(-1 * dir * 100.f, 0.f));
			}
		}
		else
			((Monster*)_other)->Hit(m_damage);
	}

	if (m_owner->GetType() == GROUP_TYPE::MONSTER && _other->GetType() == GROUP_TYPE::PLAYER)
	{
		((Player*)_other)->Hit(m_damage);
	}

	if (_other->GetType() == GROUP_TYPE::TILE && 
		(PROJECTILE_TYPE::BOSS_PROJECTILE == m_projectileType || PROJECTILE_TYPE::BOSS_SMALL_PROJECTILE == m_projectileType))
	{
		if (!IsDead() && !m_bDie)
		{
			m_bDie = true;
		}
	}
}

void Projectile::OnCollider(Obj* _other)
{
	if (_other->GetType() == GROUP_TYPE::TILE && PROJECTILE_TYPE::HEAD == m_projectileType)
	{
		Vector2 otherPos = _other->GetPos();
		Vector2 pos = GetPos();
		Vector2 tileColliderPos = _other->GetCollider()->GetFinalPos();
		Vector2 dir = pos - tileColliderPos;
		Vector2 distance = dir;
		Vector2 colliderSizeHalf = GetCollider()->GetScale() / 2.f;
		Vector2 tileColliderSize = _other->GetCollider()->GetScale() / 2.f;

		if (dir != Vector2(0.f, 0.f))
		{
			dir.normalize();
		}

		m_bHead = true;

		TILE_TYPE tileType = ((Tile*)_other)->GetTileType();

		if (TILE_TYPE::BOUNDARY == tileType)
		{
			if (dir.x > 0)
			{
				if (tileColliderSize.x / 2.f < distance.x * 2)
				{
					m_bRightWall = true;
					pos.x = tileColliderPos.x + tileColliderSize.x + colliderSizeHalf.x;
				}
				else
				{
					m_bLeftWall = true;
					pos.x = tileColliderPos.x - tileColliderSize.x - colliderSizeHalf.x;
				}
			}
			//타일의 왼쪽에서 부딪혔을 때
			else if (dir.x < 0)
			{
				m_bLeftWall = true;
				pos.x = tileColliderPos.x - tileColliderSize.x - colliderSizeHalf.x;
			}
		}
		else if (TILE_TYPE::GROUND == tileType)
		{
			// 타일의 위쪽에서 부딪혔을 때
			if (1.f >= dir.y && (-colliderSizeHalf.x < distance.x && tileColliderSize.x + colliderSizeHalf.x > distance.x) &&
				tileColliderSize.y * 0.3f > distance.y)
			{
				m_bGround = true;
				pos.y = tileColliderPos.y - tileColliderSize.y;
			}
			else if (!m_bGround && dir.x > 0)
			{
				// 타일의 오른쪽에서 부딪혔을 때
				if (tileColliderSize.x < distance.x * 2)
				{
					m_bRightWall = true;
					pos.x = otherPos.x + tileColliderSize.x + colliderSizeHalf.x;
				}
				//타일의 왼쪽에서 부딪혔을 때
				else
				{
					m_bLeftWall = true;
					pos.x = tileColliderPos.x - tileColliderSize.x - colliderSizeHalf.x;
				}
			}
			//타일의 왼쪽에서 부딪혔을 때
			else if (!m_bGround && dir.x < 0)
			{
				m_bLeftWall = true;
				pos.x = tileColliderPos.x - tileColliderSize.x - colliderSizeHalf.x;
			}
		}
		else if (TILE_TYPE::PLATFORM == tileType)
		{
			// 타일의 위쪽에서 부딪혔을 때
			if (1.f >= dir.y && (-colliderSizeHalf.x < distance.x && tileColliderSize.x + colliderSizeHalf.x > distance.x) &&
				tileColliderSize.y * 0.7f > distance.y)
			{
				m_bGround = true;
				pos.y = tileColliderPos.y - tileColliderSize.y;
			}
		}

		SetPos(pos);
	}
}

void Projectile::OnColliderExit(Obj* _other)
{
	if (_other->GetType() == GROUP_TYPE::TILE && PROJECTILE_TYPE::HEAD == m_projectileType)
	{
		if (m_bGround)
			m_bGround = false;

		if (m_bLeftWall)
			m_bLeftWall = false;

		if (m_bRightWall)
			m_bRightWall = false;
	}
}

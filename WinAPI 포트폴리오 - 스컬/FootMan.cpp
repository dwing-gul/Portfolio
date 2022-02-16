#include "stdafx.h"
#include "FootMan.h"
#include "Animator.h"
#include "Collider.h"
#include "Tile.h"
#include "AI.h"
#include "State_Idle.h"
#include "State_Trace.h"
#include "State_Attack.h"
#include "State_Hit.h"
#include "Projectile.h"
#include "SceneManager.h"
#include "Scene.h"

FootMan::FootMan() :
	m_bGround(false),
	m_bRightWall(false),
	m_bLeftWall(false),
	m_groundCount(0)
{
	Scene* curScene = SceneManager::GetInst()->GetCurScene();

	if (curScene->GetType() == SCENE_TYPE::TOOL)
	{
		CreateTexture(L"Footman", L"Texture\\Character\\footman.bmp");
	}
	else
	{
		SetHP(100);
		CreateAnimator();
		GetAnimator()->Load(L"Data\\Anim\\Footman.anim");

		SetDamage(5);

		CreateAI();
		GetAI()->AddState(L"Idle", new State_Idle);
		GetAI()->AddState(L"Trace", new State_Trace);
		GetAI()->AddState(L"Attack", new State_Attack);
		GetAI()->AddState(L"Hit", new State_Hit);

		GetAI()->ChangeState(L"Idle");
	}
}

FootMan::~FootMan()
{
}

void FootMan::update()
{
	vector<Obj*> playerProjectile = SceneManager::GetInst()->GetCurScene()->GetObjects(GROUP_TYPE::PLAYER_PROJECTILE);

	for (size_t i = 0; i < playerProjectile.size(); ++i)
	{
		if (((Projectile*)playerProjectile[i])->IsHead())
		{
			SetHit(false);
		}
	}

	if (IsHit())
	{
		GetAI()->ChangeState(L"Hit");
	}

	Monster::update();
}

void FootMan::lateUpdate()
{
	if (!m_bGround)
	{
		AddForce(Vector2(0.f, 130.f));
	}

	Monster::lateUpdate();
}

void FootMan::OnColliderEnter(Obj* _other)
{
	if (_other->GetType() == GROUP_TYPE::TILE)
	{
		Vector2 otherPos = _other->GetPos();
		Vector2 pos = GetPos();
		Vector2 dir = pos - otherPos;
		Vector2 distance = dir;
		float colliderXSizeHalf = GetCollider()->GetScale().x / 2.f;
		Vector2 tileColliderSize = _other->GetCollider()->GetScale();
		if (dir != Vector2(0.f, 0.f))
			dir.normalize();

		TILE_TYPE tileType = ((Tile*)_other)->GetTileType();

		if (TILE_TYPE::GROUND == tileType)
		{
			// 타일의 위쪽에서 부딪혔을 때
			if (1.f >= dir.y && (-colliderXSizeHalf < distance.x && tileColliderSize.x + colliderXSizeHalf > distance.x) &&
				tileColliderSize.y * 0.3f > distance.y)
			{
				m_bGround = true;
				++m_groundCount;
			}
		}

		if (TILE_TYPE::PLATFORM == tileType)
		{
			if (1.f >= dir.y && (-colliderXSizeHalf < distance.x && tileColliderSize.x + colliderXSizeHalf > distance.x) &&
				tileColliderSize.y * 0.7f > distance.y)
			{
				m_bGround = true;
				++m_groundCount;
			}
		}
	}
}

void FootMan::OnCollider(Obj* _other)
{
	if (_other->GetType() == GROUP_TYPE::TILE)
	{
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

		TILE_TYPE tileType = ((Tile*)_other)->GetTileType();

		if (TILE_TYPE::BOUNDARY == tileType)
		{
			if (dir.x > 0)
			{
				if (tileColliderSize.x / 2.f < distance.x)
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
			if (m_bGround)
			{
				pos.y = tileColliderPos.y - tileColliderSize.y;
			}
			else if (!m_bGround && dir.x > 0)
			{
				// 타일의 오른쪽에서 부딪혔을 때
				if (tileColliderSize.x / 2.f < distance.x)
				{
					m_bRightWall = true;
					pos.x = tileColliderPos.x + tileColliderSize.x + colliderSizeHalf.x;
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
			if (m_bGround)
			{
				pos.y = tileColliderPos.y - tileColliderSize.y;
			}
		}

		SetPos(pos);
	}
}

void FootMan::OnColliderExit(Obj* _other)
{
	if (_other->GetType() == GROUP_TYPE::PLAYER_HITBOX)
	{
		SetHit(false);
	}

	if (_other->GetType() == GROUP_TYPE::TILE)
	{
		TILE_TYPE tileType = ((Tile*)_other)->GetTileType();

		if (TILE_TYPE::GROUND == tileType || TILE_TYPE::PLATFORM == tileType)
		{
			if (m_bGround)
			{
				--m_groundCount;

				if (0 >= m_groundCount)
					m_bGround = false;
			}
		}

		if (m_bLeftWall)
			m_bLeftWall = false;

		if (m_bRightWall)
			m_bRightWall = false;
	}
}

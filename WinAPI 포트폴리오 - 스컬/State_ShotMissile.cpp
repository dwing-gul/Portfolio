#include "stdafx.h"
#include "State_ShotMissile.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Tile.h"
#include "Animator.h"
#include "Animation.h"
#include "TimeManager.h"
#include "Projectile.h"
#include "Collider.h"
#include "Texture.h"
#include "Boss.h"

bool State_ShotMissile::g_bShot = false;
bool State_ShotMissile::g_bFinish = false;

State_ShotMissile::State_ShotMissile() :
	m_target(nullptr),
	m_bInit(true),
	m_bReady(false),
	m_bShot(false),
	m_speed(200.f),
	m_term(1.f),
	m_count(3)
{

}

State_ShotMissile::~State_ShotMissile()
{
}

void State_ShotMissile::update()
{
	if (m_bInit)
	{
		m_prevPos = GetMonster()->GetPos();
		((Boss*)GetMonster())->SetPrevPos(m_prevPos);
		m_bInit = false;
	}

	if (!m_bReady)
	{
		vector<Obj*> tile = SceneManager::GetInst()->GetCurScene()->GetObjects(GROUP_TYPE::TILE);
		Vector2 pos = GetMonster()->GetPos();

		if (GetMonster()->GetName() == L"LeftHand" || GetMonster()->GetName() == L"RightHand")
		{
			for (size_t i = 0; i < tile.size(); ++i)
			{
				if (((Tile*)tile[i])->GetTileType() == TILE_TYPE::GROUND)
				{
					pos.y = tile[i]->GetPos().y;
					break;
				}
			}

			GetMonster()->SetPos(pos);
		}
		else if (GetMonster()->GetName() == L"BossBody")
		{
			float targetPos = 0.f;
			for (size_t i = 0; i < tile.size(); ++i)
			{
				if (((Tile*)tile[i])->GetTileType() == TILE_TYPE::GROUND)
				{
					targetPos = tile[i]->GetPos().y + 165.f;
					break;
				}
			}

			pos.y += m_speed * DELTATIME;

			if (pos.y >= targetPos)
			{
				pos.y = targetPos;
				m_bReady = true;
			}

			GetMonster()->SetPos(pos);
		}

		GetMonster()->GetAnimator()->PlayAnim(L"ReadyToProjectile", true);
	}
	else
	{
		if (GetMonster()->GetName() == L"BossBody")
		{
			Vector2 pos = GetMonster()->GetPos();
			Vector2 dir = m_prevPos - pos;
			float yDistance = abs(dir.y);

			if (!dir.IsZero())
				dir.normalize();

			if (yDistance > 3.f)
			{
				pos.y += dir.y * m_speed * DELTATIME;
				GetMonster()->SetPos(pos);
			}
			else
			{
				PlaySoundEffect(L"ShotReady", L"Sound\\ElderEnt_EnergyBomb_Ready.wav", false, 0.f, 70.f);
				g_bShot = true;
			}
		}
	}

	Scene* curScene = SceneManager::GetInst()->GetCurScene();
	vector<Obj*> missiles = curScene->GetObjects(GROUP_TYPE::MONSTER_PROJECTILE);

	if (0 != missiles.size())
	{
		m_bShot = true;
	}
}

void State_ShotMissile::lateUpdate()
{
	if (g_bShot)
	{
		if (0 < m_count)
		{
			Animation* curAnim = GetMonster()->GetAnimator()->GetCurAnim();
			if (nullptr != curAnim && curAnim->IsFinish())
			{
				if (GetMonster()->GetName() == L"BossHead" && !m_bShot)
				{
					PlaySoundEffect(L"Fire", L"Sound\\ElderEnt_EnergyBomb_Fire.wav", false, 0.f, 70.f);
					CreateProjectile(Vector2(0.f, -80.f), Vector2(10.f, 10.f), Vector2(50.f, 50.f), L"Texture\\Object\\BossSmallProjectile.bmp",
						PROJECTILE_TYPE::BOSS_SMALL_PROJECTILE, 10, Vector2(1.f, 0.f));
					CreateProjectile(Vector2(0.f, -80.f), Vector2(10.f, 10.f), Vector2(50.f, 50.f), L"Texture\\Object\\BossSmallProjectile.bmp",
						PROJECTILE_TYPE::BOSS_SMALL_PROJECTILE, 10, Vector2(-1.f, 0.f));
					CreateProjectile(Vector2(0.f, -80.f), Vector2(10.f, 10.f), Vector2(50.f, 50.f), L"Texture\\Object\\BossSmallProjectile.bmp",
						PROJECTILE_TYPE::BOSS_SMALL_PROJECTILE, 10, Vector2(0.f, 1.f));
					CreateProjectile(Vector2(0.f, -80.f), Vector2(10.f, 10.f), Vector2(50.f, 50.f), L"Texture\\Object\\BossSmallProjectile.bmp",
						PROJECTILE_TYPE::BOSS_SMALL_PROJECTILE, 10, Vector2(0.f, -1.f));
					CreateProjectile(Vector2(0.f, -80.f), Vector2(10.f, 10.f), Vector2(50.f, 50.f), L"Texture\\Object\\BossSmallProjectile.bmp",
						PROJECTILE_TYPE::BOSS_SMALL_PROJECTILE, 10, Vector2(sqrtf(2), sqrtf(2)));
					CreateProjectile(Vector2(0.f, -80.f), Vector2(10.f, 10.f), Vector2(50.f, 50.f), L"Texture\\Object\\BossSmallProjectile.bmp",
						PROJECTILE_TYPE::BOSS_SMALL_PROJECTILE, 10, Vector2(-sqrtf(2), sqrtf(2)));
					CreateProjectile(Vector2(0.f, -80.f), Vector2(10.f, 10.f), Vector2(50.f, 50.f), L"Texture\\Object\\BossSmallProjectile.bmp",
						PROJECTILE_TYPE::BOSS_SMALL_PROJECTILE, 10, Vector2(sqrtf(2), -sqrtf(2)));
					CreateProjectile(Vector2(0.f, -80.f), Vector2(10.f, 10.f), Vector2(50.f, 50.f), L"Texture\\Object\\BossSmallProjectile.bmp",
						PROJECTILE_TYPE::BOSS_SMALL_PROJECTILE, 10, Vector2(-sqrtf(2), -sqrtf(2)));
					CreateProjectile(Vector2(0.f, -80.f), Vector2(20.f, 20.f), Vector2(80.f, 80.f), L"Texture\\Object\\BossProjectile.bmp",
						PROJECTILE_TYPE::BOSS_PROJECTILE, 30, m_target->GetPos());
					
					m_bShot = true;
					--m_count;
				}
			}

			Scene* curScene = SceneManager::GetInst()->GetCurScene();
			vector<Obj*> missiles = curScene->GetObjects(GROUP_TYPE::MONSTER_PROJECTILE);
		
			if (0 == missiles.size())
			{
				m_bShot = false;
			}
		}
		else
		{
			g_bFinish = true;
		}
	}

	if (g_bFinish)
	{
		m_term -= DELTATIME;

		if(m_term < 0.f)
			ChangeState(L"Groggy");
	}
}

void State_ShotMissile::Enter()
{
	if (nullptr == m_target)
	{
		m_target = SceneManager::GetInst()->GetObjectByName(L"Player", GROUP_TYPE::PLAYER);
	}

	g_bFinish = false;
	g_bShot = false;
}

void State_ShotMissile::Exit()
{
	m_bInit = true;
	m_bReady = false;
	m_bShot = false;
	m_count = 3;
	m_term = 1.f;
}

void State_ShotMissile::CreateProjectile(Vector2 _offset, Vector2 _colliderOffset, Vector2 _scale, const wstring& _relativePath, PROJECTILE_TYPE _type, int _damage, Vector2 _dir)
{
	if (PROJECTILE_TYPE::BOSS_PROJECTILE == _type)
	{
		// 발사 직전에 플레이어 위치를 받아와서 그 방향으로 움직여야됨
		Projectile* small = new Projectile;
		small->SetOffsetPos(_offset);
		small->CreateTexture(L"BossProjectile", _relativePath);
		float width = (float)small->GetTexture()->GetWidth();
		float height = (float)small->GetTexture()->GetHeight();
		small->SetScale(Vector2(width, height));
		small->SetDamage(_damage);
		small->SetOwner(GetMonster());
		small->CreateCollider();
		small->GetCollider()->SetOffsetPos(_colliderOffset);
		small->GetCollider()->SetScale(_scale);

		if (_dir.y < GetMonster()->GetPos().y)
		{
			_dir.y = GetMonster()->GetPos().y;
		}

		small->SetDir(_dir);
		small->SetType(GROUP_TYPE::MONSTER_PROJECTILE);
		small->SetProjectileType(_type);

		CreateObject(small, GROUP_TYPE::MONSTER_PROJECTILE);
	}
	else if (PROJECTILE_TYPE::BOSS_SMALL_PROJECTILE == _type)
	{
		Projectile* small = new Projectile;
		small->SetOffsetPos(_offset);
		small->CreateTexture(L"BossSmallProjectile", _relativePath);
		float width = (float)small->GetTexture()->GetWidth();
		float height = (float)small->GetTexture()->GetHeight();
		small->SetScale(Vector2(width, height));
		small->SetDamage(_damage);
		small->SetOwner(GetMonster());
		small->CreateCollider();
		small->GetCollider()->SetOffsetPos(_colliderOffset);
		small->GetCollider()->SetScale(_scale);
		small->SetDir(_dir);
		small->SetType(GROUP_TYPE::MONSTER_PROJECTILE);
		small->SetProjectileType(_type);

		CreateObject(small, GROUP_TYPE::MONSTER_PROJECTILE);
	}
}

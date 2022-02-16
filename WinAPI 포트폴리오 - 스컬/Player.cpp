#include "stdafx.h"
#include "Player.h"
#include "Animator.h"
#include "Animation.h"
#include "Camera.h"
#include "Collider.h"
#include "TimeManager.h"
#include "KeyManager.h"
#include "Texture.h"
#include "Tile.h"
#include "HitBox.h"
#include "Projectile.h"
#include "Item.h"
#include "Sound.h"
#include "ResourceManager.h"
#include "Effect.h"
#include "SceneManager.h"
#include "Scene.h"

int Player::g_maxHP = 9000;
int Player::g_curHP = g_maxHP;
int Player::g_physicalDamage = 20;
int Player::g_magicalDamage = 10;
float Player::g_shiftCoolTime = 10.f;
SKUL Player::g_curSkul = SKUL::BASIC;

Player::Player() :
	m_grade(GRADE::NORMAL),
	m_attackType(ATTACK_TYPE::PHYSICAL),
	m_speed(300.f),
	m_speedRate(1.f),
	m_attackSpeed(1.f),
	m_shiftCoolTime(10.f),
	m_dashCoolTime(2.f),
	m_damageReduction(0.f),
	m_passiveSkilTime(3.f),
	m_hpRatio(1.f),
	m_criticalRate(30.f),
	m_criticalRatio(1.f),
	m_duration(0.f),
	m_bGround(false),
	m_bLeftWall(false),
	m_bRightWall(false),
	m_bDash(false),
	m_bKeepDash(false),
	m_bDie(false),
	m_bCritical(false),
	m_bHit(false),
	m_bJump(false),
	m_bAttack(false),
	m_bAdapt(false),
	m_bSkilAdapt(false),
	m_bFinish(false),
	m_bFirst(true),
	m_jumpCount(JUMP_COUNT),
	m_maxDashCount(2),
	m_curDashCount(m_maxDashCount),
	m_dir(1),
	m_groundCount(0),
	m_hitBox(nullptr),
	m_projectile(nullptr),
	m_vectorItem{},
	m_vectorSkul{},
	m_vectorSkilCool{},
	m_essense(nullptr),
	m_bHeadless(false)
{
	CreateCollider();
	Vector2 pos = GetPos();
	GetCollider()->SetOffsetPos(Vector2(pos.x, pos.y - 25.f));
	GetCollider()->SetScale(Vector2(50.f, 50.f));
}

Player::~Player()
{
	if (0 != m_vectorItem.size())
	{
		for (size_t i = 0; i < m_vectorItem.size(); ++i)
		{
			delete m_vectorItem[i];
		}

		m_vectorItem.clear();
	}

	if (nullptr != m_essense)
		delete m_essense;
}

void Player::update()
{
	m_hpRatio = float(g_curHP) / float(g_maxHP);
	m_bCritical = false;

	if (g_maxHP > 9999)
	{
		g_maxHP = 9999;
		g_curHP = int((float)g_maxHP * m_hpRatio);
	}

	wstring curAnimName = GetAnimator()->GetCurAnimName();
	Vector2 pos = GetPos();
	
	if (!m_bDie)
	{
		if (g_shiftCoolTime > 0.f)
		{
			g_shiftCoolTime -= m_shiftCoolTime * DELTATIME;
		}
		else
		{
			g_shiftCoolTime = 0.f;
		}

		if (!m_bGround)
		{
			if (!m_bHeadless)
			{
				ChangeAnim(L"Jump_Attack", L"Fall", m_dir);
				ChangeAnim(L"Jump", L"Fall_Ready", m_dir);
				ChangeAnim(L"Fall_Ready", L"Fall", m_dir);
			}
			else
			{
				ChangeAnim(L"Headless_Jump_Attack", L"Headless_Fall", m_dir);
				ChangeAnim(L"Headless_Jump", L"Headless_Fall_Ready", m_dir);
				ChangeAnim(L"Headless_Fall_Ready", L"Headless_Fall", m_dir);
			}

			if (KEY_HOLD(KEY::LEFT) && !m_bRightWall && !m_bDash)
			{
				pos.x -= m_speed * DELTATIME;
				m_dir = -1;

				if (!m_bHeadless)
				{
					if (!m_bJump)
					{
						SetAnimDir(m_dir, L"Fall", true);
					}
					else
					{
						if (curAnimName == L"Dash_Right" || curAnimName == L"Dash_Left")
							SetAnimDir(m_dir, L"Fall", true);
						else
						{
							ChangeAnim(L"Jump", L"Fall_Ready", m_dir);
							ChangeAnim(L"Fall_Ready", L"Fall", m_dir);

							if (curAnimName == L"Fall_Right")
							{
								SetAnimDir(m_dir, L"Fall", true);
							}
						}
					}
				}
				else
				{
					if (!m_bJump)
					{
						SetAnimDir(m_dir, L"Headless_Fall", true);
					}
					else
					{
						ChangeAnim(L"Headless_Jump", L"Headless_Fall_Ready", m_dir);
						ChangeAnim(L"Headless_Fall_Ready", L"Headless_Fall", m_dir);
						if (curAnimName == L"Headless_Fall_Right")
						{
							SetAnimDir(m_dir, L"Headless_Fall", true);
						}
					}
				}
			}

			if (KEY_HOLD(KEY::RIGHT) && !m_bLeftWall && !m_bDash)
			{
				pos.x += m_speed * DELTATIME;
				m_dir = 1;

				if (!m_bHeadless)
				{
					if (!m_bJump)
					{
						SetAnimDir(m_dir, L"Fall", true);
					}
					else
					{
						if (curAnimName == L"Dash_Right" || curAnimName == L"Dash_Left")
							SetAnimDir(m_dir, L"Fall", true);
						else
						{
							ChangeAnim(L"Jump", L"Fall_Ready", m_dir);
							ChangeAnim(L"Fall_Ready", L"Fall", m_dir);
							if (curAnimName == L"Fall_Left")
							{
								SetAnimDir(m_dir, L"Fall", true);
							}
						}
					}

					ChangeAnim(L"Jump_Attack", L"Fall", m_dir);
				}
				else
				{
					if (!m_bJump)
					{
						SetAnimDir(m_dir, L"Headless_Fall", true);
					}
					else
					{
						ChangeAnim(L"Headless_Jump", L"Headless_Fall_Ready", m_dir);
						ChangeAnim(L"Headless_Fall_Ready", L"Headless_Fall", m_dir);
						if (curAnimName == L"Headless_Fall_Left")
						{
							SetAnimDir(m_dir, L"Headless_Fall", true);
						}
					}
				}

				ChangeAnim(L"Headless_Jump_Attack", L"Headless_Fall", m_dir);
			}
		}

		if (KEY_TAP(KEY::C) && 0 < m_jumpCount && KEY_NONE(KEY::DOWN) && !m_bDash)
		{
			AddVelocity(Vector2(0.f, -400.f));
			AddForce(Vector2(0.f, -400.f));
			--m_jumpCount;

			if (!m_bHeadless)
				SetAnimDir(m_dir, L"Jump", true);
			else
				SetAnimDir(m_dir, L"Headless_Jump", true);

			m_bGround = false;
			m_groundCount = 0;
			m_bJump = true;
			PlaySoundEffect(L"Jump", L"Sound\\Jump.wav", false, 0.f, 30.f);
		}

		SetPos(pos);
	}

	if (!m_bHeadless)
	{
		if (nullptr != m_hitBox && (curAnimName != L"Attack_Right" && curAnimName != L"Attack_Left" &&
			curAnimName != L"Jump_Attack_Right" && curAnimName != L"Jump_Attack_Left" && curAnimName != L"MadAxe_Right" && curAnimName != L"MadAxe_Left"))
		{
			DeleteObject(m_hitBox);
			m_hitBox = nullptr;
		}

		if (curAnimName == L"MadAxe_Right" || curAnimName == L"MadAxe_Left")
		{
			if (GetAnimator()->GetCurAnim()->GetCurFrameIdx() == 5)
			{
				Vector2 pos = GetPos();
				pos.x += m_dir * 60.f;

				Effect* effect = new Effect;
				effect->CreateAnimator();
				effect->GetAnimator()->Load(L"Data\\Anim\\Stamp_Effect.anim");
				effect->GetAnimator()->PlayAnim(L"Idle", false);
				effect->SetPos(pos);
				effect->SetMagnification(Vector2(0.5f, 0.5f));
				CreateObject(effect, GROUP_TYPE::EFFECT);
			}
		}

		if (curAnimName != L"Attack_Right" && curAnimName != L"Attack_Left" &&
			curAnimName != L"Jump_Attack_Right" && curAnimName != L"Jump_Attack_Left")
		{
			m_bAttack = false;
		}

		if ((curAnimName == L"Jump_Attack_Right" || curAnimName == L"Jump_Attack_Left") && m_bGround)
		{
			SetAnimDir(m_dir, L"Idle", true);
		}

		if (curAnimName == L"Attack_Right" || curAnimName == L"Attack_Left")
		{
			if (GetAnimator()->GetCurAnim()->GetCurFrameIdx() == 5)
				PlaySoundEffect(L"Attack", L"Sound\\Skul_Atk.wav", false, 0.f, 30.f);
		}

		if (m_bGround && (curAnimName == L"Fall_Right" || curAnimName == L"Fall_Left"))
			SetAnimDir(m_dir, L"Idle", true);

		if ((curAnimName == L"Idle_Right" || curAnimName == L"Idle_Left") && !m_bGround)
			SetAnimDir(m_dir, L"Fall", true);

		ChangeAnim(L"Attack", L"Idle", m_dir);
		ChangeAnim(L"Dash", L"Idle", m_dir);

		ChangeAnim(L"MadAxe", L"Idle", m_dir);
	}
	else
	{
		if (nullptr != m_hitBox && (curAnimName != L"Headless_Attack_Right" && curAnimName != L"Headless_Attack_Left") &&
			(curAnimName != L"Headless_Jump_Attack_Right" && curAnimName != L"Headless_Jump_Attack_Left"))
		{
			DeleteObject(m_hitBox);
			m_hitBox = nullptr;
		}

		if (curAnimName != L"Headless_Attack_Right" && curAnimName != L"Headless_Attack_Left" &&
			curAnimName != L"Headless_Jump_Attack_Right" && curAnimName != L"Headless_Jump_Attack_Left")
		{
			m_bAttack = false;
		}

		if (curAnimName == L"Headless_Attack_Right" || curAnimName == L"Headless_Attack_Left")
		{
			if (GetAnimator()->GetCurAnim()->GetCurFrameIdx() == 5)
				PlaySoundEffect(L"Attack", L"Sound\\Skul_Atk.wav", false, 0.f, 30.f);
		}

		if ((curAnimName == L"Headless_Jump_Attack_Right" || curAnimName == L"Headless_Jump_Attack_Left") && m_bGround)
		{
			SetAnimDir(m_dir, L"Headless_Idle", true);
		}

		if (m_bGround && (curAnimName == L"Headless_Fall_Right" || curAnimName == L"Headless_Fall_Left"))
			SetAnimDir(m_dir, L"Headless_Idle", true);

		if ((curAnimName == L"Headless_Idle_Right" || curAnimName == L"Headless_Idle_Left") && !m_bGround)
			SetAnimDir(m_dir, L"Headless_Fall", true);

		ChangeAnim(L"Headless_Attack", L"Headless_Idle", m_dir);
		ChangeAnim(L"Headless_Dash", L"Headless_Idle", m_dir);
	}

	if (0 != m_vectorItem.size())
	{
		for (size_t i = 0; i < m_vectorItem.size(); ++i)
		{
			m_vectorItem[i]->update();
		}
	}

	if (nullptr != m_essense)
		m_essense->update();

	if (KEY_TAP(KEY::SPACE) && 2 == m_vectorSkul.size() && g_shiftCoolTime <= 0.f)
	{
		SKUL temp = m_vectorSkul[0];
		m_vectorSkul[0] = m_vectorSkul[1];
		m_vectorSkul[1] = temp;

		g_curSkul = m_vectorSkul[0];
		ChangeSkul();

		Scene* curScene = SceneManager::GetInst()->GetCurScene();

		curScene->UIChange(this);

		g_shiftCoolTime = 10.f;
	}

	for (size_t i = 0; i < m_vectorSkilCool.size(); ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			if (m_vectorSkilCool[i].m_bSkilUse[j])
			{
				m_vectorSkilCool[i].m_skilCoolTime[j] -= DELTATIME;

				if (m_vectorSkilCool[i].m_skilCoolTime[j] <= 0.f)
				{
					m_vectorSkilCool[i].m_bSkilUse[j] = false;
				}
			}
		}
	}

	RigidBody::update();
}

void Player::lateUpdate()
{
	Skil();

	Vector2 pos = GetPos();
	srand((unsigned int)time(NULL));

	if (!m_bGround && !m_bDash)
	{
		AddForce(Vector2(0.f, 250.f));

		if (!m_bDie)
		{
			if (KEY_TAP(KEY::Z) && 0 < m_curDashCount)
			{
				m_bDash = true;
				m_bKeepDash = true;

				if(!m_bHeadless)
					SetAnimDir(m_dir, L"Dash", true);
				else
					SetAnimDir(m_dir, L"Headless_Dash", true);

				--m_curDashCount;

				PlaySoundEffect(L"Dash", L"Sound\\Dash.wav", false, 0.f, 30.f);
			}

			if (KEY_TAP(KEY::X) && !m_bDash && !m_bAttack)
			{
				m_bAttack = true;
				int temp = rand();
				float percent = rand() % 10000 / 100.f;
				int damage = 0;

				if(!m_bHeadless)
					SetAnimDir(m_dir, L"Jump_Attack", true);
				else
					SetAnimDir(m_dir, L"Headless_Jump_Attack", true);

				PlaySoundEffect(L"Attack", L"Sound\\Skul_Atk.wav", false, 0.f, 30.f);

				GetAnimator()->GetCurAnim()->SetAnimSpeed(m_attackSpeed);

				if (ATTACK_TYPE::PHYSICAL == m_attackType)
					damage = g_physicalDamage;
				else
					damage = g_magicalDamage;

				if (percent > m_criticalRate * m_criticalRatio)
				{
					m_bCritical = true;
				}

				if (m_bCritical)
					damage *= 2;

				CreateHitBox(Vector2(m_dir * 20.f, -50.f), Vector2(m_dir * 40.f, -50.f), Vector2(100.f, 100.f), damage);
			}
		}
	}
	else
	{
		if (!m_bDie)
		{
			if(!m_bHeadless)
				ChangeAnim(L"Jump_Attack", L"Idle", m_dir);
			else
				ChangeAnim(L"Headless_Jump_Attack", L"Headless_Idle", m_dir);

			wstring curAnimName = GetAnimator()->GetCurAnimName();

			if (!m_bHeadless)
			{
				if (KEY_HOLD(KEY::LEFT) && !m_bRightWall && (curAnimName != L"Attack_Right" && curAnimName != L"Attack_Left") && !m_bDash)
				{
					pos.x -= m_speed * DELTATIME;
					float a = DELTATIME;
					m_dir = -1;

					SetAnimDir(m_dir, L"Walk", true);
				}
				if (KEY_AWAY(KEY::LEFT) && !m_bDash)
				{
					SetAnimDir(m_dir, L"Idle", true);
				}

				if (KEY_HOLD(KEY::RIGHT) && !m_bLeftWall && (curAnimName != L"Attack_Right" && curAnimName != L"Attack_Left") && !m_bDash)
				{
					pos.x += m_speed * DELTATIME;
					m_dir = 1;

					SetAnimDir(m_dir, L"Walk", true);
				}
				if (KEY_AWAY(KEY::RIGHT) && !m_bDash)
				{
					SetAnimDir(m_dir, L"Idle", true);
				}
			}
			else
			{
				if (KEY_HOLD(KEY::LEFT) && !m_bRightWall && (curAnimName != L"Headless_Attack_Right" && curAnimName != L"Headless_Attack_Left") && !m_bDash)
				{
					pos.x -= m_speed * m_speedRate * DELTATIME;
					m_dir = -1;
					SetAnimDir(m_dir, L"Headless_Walk", true);
					GetAnimator()->GetCurAnim()->SetAnimSpeed(m_speedRate);
				}
				if (KEY_AWAY(KEY::LEFT))
				{
					SetAnimDir(m_dir, L"Headless_Idle", true);
				}

				if (KEY_HOLD(KEY::RIGHT) && !m_bLeftWall && (curAnimName != L"Headless_Attack_Right" && curAnimName != L"Headless_Attack_Left") && !m_bDash)
				{
					pos.x += m_speed * m_speedRate * DELTATIME;
					m_dir = 1;

					SetAnimDir(m_dir, L"Headless_Walk", true);
					GetAnimator()->GetCurAnim()->SetAnimSpeed(m_speedRate);
				}
				if (KEY_AWAY(KEY::RIGHT))
				{
					SetAnimDir(m_dir, L"Headless_Idle", true);
				}
			}

			if (KEY_TAP(KEY::X) && !m_bDash && !m_bAttack)
			{
				m_bAttack = true;
				int temp = rand();
				float percent = rand() % 10001 / 100.f;
				int damage = 0;

				if(!m_bHeadless)
					SetAnimDir(m_dir, L"Attack", true);
				else
					SetAnimDir(m_dir, L"Headless_Attack", true);

				GetAnimator()->GetCurAnim()->SetAnimSpeed(m_attackSpeed);
				PlaySoundEffect(L"Attack1", L"Sound\\Skul_Atk1.wav", false, 0.f, 30.f);

				if (ATTACK_TYPE::PHYSICAL == m_attackType)
					damage = g_physicalDamage;
				else
					damage = g_magicalDamage;

				if (percent < m_criticalRate * m_criticalRatio)
				{
					m_bCritical = true;
				}

				if (m_bCritical)
					damage *= 2;

				CreateHitBox(Vector2(m_dir * 20.f, -50.f), Vector2(m_dir * 40.f, -50.f), Vector2(100.f, 100.f), damage);
			}

			if (KEY_TAP(KEY::Z) && 0 < m_curDashCount)
			{
				m_bDash = true;
				m_bKeepDash = true;
				--m_curDashCount;

				if(!m_bHeadless)
					SetAnimDir(m_dir, L"Dash", true);
				else
					SetAnimDir(m_dir, L"Headless_Dash", true);

				PlaySoundEffect(L"Dash", L"Sound\\Dash.wav", false, 0.f, 30.f);
			}
		}
	}

	if (m_curDashCount == 0)
	{
		m_dashCoolTime -= DELTATIME;

		if (m_dashCoolTime < 0.f)
		{
			m_curDashCount = m_maxDashCount;
			m_dashCoolTime = 2.f;
		}
	}

	if (m_bDash)
	{
		if (m_bKeepDash)
		{
			SetVelocity(Vector2(m_dir * 1700.f, 0.f));
			SetSubSpeed(275.f);
			m_bKeepDash = false;
		}

		if (GetVelocity() == Vector2(0.f, 0.f))
		{
			m_bDash = false;

			if (nullptr != m_hitBox)
			{
				DeleteObject(m_hitBox);
				m_hitBox = nullptr;
			}
		}
	}

	if (g_curHP <= 0)
	{
		g_curHP = 0;
		m_bDie = true;
		SetAnimDir(m_dir, L"Dead", false);
	}

	SetPos(pos);

	if (0 != m_vectorItem.size())
	{
		for (size_t i = 0; i < m_vectorItem.size(); ++i)
		{
			m_vectorItem[i]->lateUpdate();
		}
	}

	if (nullptr != m_essense)
		m_essense->lateUpdate();

	if(!m_bFirst)
		RigidBody::lateUpdate();

	m_bFirst = false;
}

void Player::render(HDC _dc)
{
	if (GetAnimator())
	{
		GetAnimator()->render(_dc);
	}

	Obj::render(_dc);
}

void Player::Hit(int _damage)
{
	int damage = 0;
	m_bHit = true;
	
	if(!m_bDash)
		damage = _damage - int((float)_damage * m_damageReduction);

	g_curHP -= damage;
}

void Player::CreateHitBox(Vector2 _offset, Vector2 _effectOffset, Vector2 _scale, int _damage)
{
	if (m_hitBox == nullptr)
	{
		m_hitBox = new HitBox;
		m_hitBox->m_owner = this;
		m_hitBox->SetOffset(_offset);
		m_hitBox->SetEffectOffset(_effectOffset);
		m_hitBox->SetScale(_scale);
		m_hitBox->SetDamage(_damage);
		CreateObject(m_hitBox, GROUP_TYPE::PLAYER_HITBOX);
	}
}

void Player::CreateProjectile(Vector2 _offset, Vector2 _scale, const wstring& _relativePath, PROJECTILE_TYPE _type, ATTACK_TYPE _attackType, int _damage)
{
	Vector2 pos = GetPos();

	if (PROJECTILE_TYPE::HEAD == _type)
	{
		pos.y -= 15.f;
		m_projectile = new Projectile;
		m_projectile->SetPos(pos);
		m_projectile->CreateAnimator();
		m_projectile->GetAnimator()->Load(_relativePath);
		m_projectile->GetAnimator()->PlayAnim(L"Idle", true);
		m_projectile->CreateCollider();
		m_projectile->GetCollider()->SetOffsetPos(_offset);
		m_projectile->GetCollider()->SetScale(_scale);
		m_projectile->SetDamage(_damage);
		m_projectile->m_owner = this;
		m_projectile->m_projectileType = _type;
		m_projectile->m_bFirstShot = true;
		m_projectile->SetName(L"Skull");

		CreateObject(m_projectile, GROUP_TYPE::PLAYER_PROJECTILE);
	}
	else
	{
		Projectile* projectile = new Projectile;
		projectile->SetPos(pos);
		projectile->CreateAnimator();
		projectile->GetAnimator()->Load(_relativePath);
		projectile->GetAnimator()->PlayAnim(L"Idle", true);
		projectile->CreateCollider();
		projectile->GetCollider()->SetOffsetPos(_offset);
		projectile->GetCollider()->SetScale(_scale);
		projectile->SetDamage(_damage);
		projectile->m_owner = this;

		CreateObject(projectile, GROUP_TYPE::PLAYER_PROJECTILE);
	}
}

void Player::AddItem(Item* _item)
{
	if(m_vectorItem.size() != 9)
	{
		m_vectorItem.push_back(_item);
	}
}

void Player::AddSkul(SKUL _skul)
{
	if (m_vectorSkul.size() != 2)
	{
		m_vectorSkul.push_back(_skul);
		if (SKUL::BERSERKER == _skul)
		{
			m_vectorSkilCool.push_back(tSkilCool{ {30.f, 14.f}, {false, false} });
		}
	}
}

void Player::AddEssense(Item* _essense)
{
	if (nullptr == m_essense)
	{
		m_essense = _essense->Clone();
	}
}

void Player::SkulInit()
{
	if (0 == m_vectorSkul.size())
	{
		m_vectorSkul.push_back(SKUL::BASIC);
		m_vectorSkilCool.push_back(tSkilCool{ { 6.f, 3.f }, {false, false} });
	}
}

void Player::SetAnimDir(int _dir, wstring _state, bool _repeat)
{
	if (_dir == 1)
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

void Player::ChangeAnim(wstring _curName, wstring _nextName, int _dir)
{
	if (1 == _dir)
	{
		_curName += L"_Right";
		_nextName += L"_Right";
	}
	else
	{
		_curName += L"_Left";
		_nextName += L"_Left";
	}

	wstring curAnimName = GetAnimator()->GetCurAnimName();
	Animation* curAnim = GetAnimator()->GetCurAnim();

	if (_curName == curAnimName)
	{
		curAnim->ChangeAnim(_nextName);
	}
}

void Player::ChangeSkul()
{
	if (SKUL::BERSERKER == g_curSkul)
	{
		DeleteAnimator();
		CreateAnimator();
		GetAnimator()->Load(L"Data\\Anim\\Skul_Berserker.anim");
		SetAnimDir(m_dir, L"Idle", true);
		m_maxDashCount = 1;
		
		if (0 > m_curDashCount)
			m_curDashCount = m_maxDashCount;

		m_attackType = ATTACK_TYPE::PHYSICAL;
	}
	else if (SKUL::BASIC == g_curSkul)
	{
		DeleteAnimator();
		CreateAnimator();
		GetAnimator()->Load(L"Data\\Anim\\Skul_Basic.anim");
		SetAnimDir(m_dir, L"Idle", true);
		m_maxDashCount = 2;

		if (0 > m_curDashCount)
			m_curDashCount = m_maxDashCount;

		m_attackType = ATTACK_TYPE::PHYSICAL;
	}
}

void Player::OnColliderEnter(Obj* _other)
{
	if (_other->GetType() == GROUP_TYPE::PLAYER_PROJECTILE)
	{
		if (((Projectile*)_other)->IsHead())
		{
			m_bHeadless = false;
			if (nullptr != m_projectile)
			{
				DeleteObject(m_projectile);
				m_projectile = nullptr;
			}

			m_vectorSkilCool[0].m_bSkilUse[0] = false;
			m_vectorSkilCool[0].m_skilCoolTime[0] = 6.f;

			SetAnimDir(m_dir, L"Idle", true);
		}
	}

	if (_other->GetType() == GROUP_TYPE::TILE)
	{
		Vector2 otherPos = _other->GetPos();
		Vector2 pos = GetPos();
		Vector2 dir = pos - otherPos;
		Vector2 distance = dir;
		float playerColliderXSizeHalf = GetCollider()->GetScale().x / 2.f;
		Vector2 tileColliderSize = _other->GetCollider()->GetScale();
		if (dir != Vector2(0.f, 0.f))
			dir.normalize();

		TILE_TYPE tileType = ((Tile*)_other)->GetTileType();

		if (TILE_TYPE::GROUND == tileType)
		{
			// Å¸ÀÏÀÇ À§ÂÊ¿¡¼­ ºÎµúÇûÀ» ¶§
			if (1.f >= dir.y && (-playerColliderXSizeHalf < distance.x && tileColliderSize.x + playerColliderXSizeHalf > distance.x) &&
				tileColliderSize.y  * 0.3f > distance.y)
			{
				if (!m_bGround)
					PlaySoundEffect(L"Landing", L"Sound\\Landing.wav", false, 0.f, 30.f);

				m_bGround = true;
				m_jumpCount = JUMP_COUNT;
				m_bJump = false;
				++m_groundCount;
			}
		}

		if (TILE_TYPE::PLATFORM == tileType)
		{
			if (1.f >= dir.y && (-playerColliderXSizeHalf < distance.x && tileColliderSize.x + playerColliderXSizeHalf > distance.x) &&
				tileColliderSize.y * 0.7f > distance.y)
			{
				if(!m_bGround)
					PlaySoundEffect(L"Landing", L"Sound\\Landing.wav", false, 0.f, 30.f);

				m_bGround = true;
				m_jumpCount = JUMP_COUNT;
				m_bJump = false;
				++m_groundCount;
			}
		}
	}
}

void Player::OnCollider(Obj* _other)
{
	if (_other->GetType() == GROUP_TYPE::TILE)
	{
		Vector2 pos = GetPos();
		Vector2 tileColliderPos = _other->GetCollider()->GetFinalPos();
		Vector2 dir = pos - tileColliderPos;
		Vector2 distance = dir;
		Vector2 playerColliderSizeHalf = GetCollider()->GetScale() / 2.f;
		Vector2 tileColliderSize = _other->GetCollider()->GetScale() / 2.f;

		if(dir != Vector2(0.f, 0.f))
			dir.normalize();

		TILE_TYPE tileType = ((Tile*)_other)->GetTileType();

		if (TILE_TYPE::BOUNDARY == tileType)
		{
			if (dir.x > 0)
			{
				if (tileColliderSize.x / 2.f < distance.x * 2.f)
				{
					m_bRightWall = true;
					pos.x = tileColliderPos.x + tileColliderSize.x + playerColliderSizeHalf.x;
				}
				else
				{
					m_bLeftWall = true;
					pos.x = tileColliderPos.x - tileColliderSize.x - playerColliderSizeHalf.x;
				}
			}
			//Å¸ÀÏÀÇ ¿ÞÂÊ¿¡¼­ ºÎµúÇûÀ» ¶§
			else if (dir.x < 0)
			{
				m_bLeftWall = true;
				pos.x = tileColliderPos.x - tileColliderSize.x - playerColliderSizeHalf.x;
			}
		}
		else if (TILE_TYPE::GROUND == tileType)
		{
			// Å¸ÀÏÀÇ À§ÂÊ¿¡¼­ ºÎµúÇûÀ» ¶§
			if (m_bGround)
			{
				pos.y = tileColliderPos.y - tileColliderSize.y;
			}
			else if (!m_bGround && dir.x > 0)
			{
				// Å¸ÀÏÀÇ ¿À¸¥ÂÊ¿¡¼­ ºÎµúÇûÀ» ¶§
				if (tileColliderSize.x / 2.f < distance.x * 2.f)
				{
					m_bRightWall = true;
					pos.x = tileColliderPos.x + tileColliderSize.x + playerColliderSizeHalf.x;
				}
				//Å¸ÀÏÀÇ ¿ÞÂÊ¿¡¼­ ºÎµúÇûÀ» ¶§
				else
				{
					m_bLeftWall = true;
					pos.x = tileColliderPos.x - tileColliderSize.x - playerColliderSizeHalf.x;
				}
			}
			//Å¸ÀÏÀÇ ¿ÞÂÊ¿¡¼­ ºÎµúÇûÀ» ¶§
			else if (!m_bGround && dir.x < 0)
			{
				m_bLeftWall = true;
				pos.x = tileColliderPos.x - tileColliderSize.x - playerColliderSizeHalf.x;
			}
			else
			{
				int a = 0;
			}
		}
		else if (TILE_TYPE::PLATFORM == tileType)
		{
			if (KEY_HOLD(KEY::DOWN) && KEY_TAP(KEY::C))
			{
				m_bGround = false;
			}

			// Å¸ÀÏÀÇ À§ÂÊ¿¡¼­ ºÎµúÇûÀ» ¶§
			if (m_bGround)
			{
				pos.y = tileColliderPos.y - tileColliderSize.y;
			}	
		}

		SetPos(pos);
	}
}

void Player::OnColliderExit(Obj* _other)
{
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

void Player::Skil()
{
	if (SKUL::BASIC == g_curSkul)
	{
		if (KEY_TAP(KEY::A))
		{
			if (!m_bHeadless && !m_vectorSkilCool[0].m_bSkilUse[0])
			{
				CreateProjectile(Vector2(0.f, -7.f), Vector2(17.f, 15.f), L"Data\\Anim\\BasicSkul.anim",
					PROJECTILE_TYPE::HEAD, ATTACK_TYPE::MAGICAL, g_magicalDamage);
				m_bHeadless = true;
				m_vectorSkilCool[0].m_bSkilUse[0] = true;
				m_vectorSkilCool[0].m_skilCoolTime[0] = 6.f;
				SetAnimDir(m_dir, L"Headless_Idle", true);
			}
		}

		if (KEY_TAP(KEY::S))
		{
			if (m_bHeadless && !m_vectorSkilCool[0].m_bSkilUse[1])
			{
				Vector2 pos = m_projectile->GetPos();

				SetPos(pos);
				m_bHeadless = false;

				if (nullptr != m_projectile)
				{
					DeleteObject(m_projectile);
					m_projectile = nullptr;
				}

				m_vectorSkilCool[0].m_bSkilUse[1] = true;
				m_vectorSkilCool[0].m_skilCoolTime[1] = 3.f;

				m_vectorSkilCool[0].m_bSkilUse[0] = false;
				m_vectorSkilCool[0].m_skilCoolTime[0] = 6.f;

				SetAnimDir(m_dir, L"Idle", true);
			}
		}
	}
	else if (SKUL::BERSERKER == g_curSkul)
	{
		if (m_bHit)
		{
			m_passiveSkilTime -= DELTATIME;
			
			if (!m_bAdapt)
			{
				m_attackSpeed *= 1.5f;
				m_bAdapt = true;
			}

			if (m_passiveSkilTime < 0.f)
			{
				m_passiveSkilTime = 3.f;
				m_attackSpeed /= 1.5f;
				m_bHit = false;
				m_bAdapt = false;
			}
		}

		if (KEY_TAP(KEY::A) && !m_vectorSkilCool[1].m_bSkilUse[0])
		{
			m_vectorSkilCool[1].m_bSkilUse[0] = true;
			m_vectorSkilCool[1].m_skilCoolTime[0] = 30.f;
		}

		if (m_vectorSkilCool[1].m_bSkilUse[0])
		{
			m_duration += DELTATIME;
			
			if (!m_bSkilAdapt)
			{
				m_attackSpeed += 1.f;
				m_bSkilAdapt = true;
			}

			if (m_duration > 10.f && !m_bFinish)
			{
				m_attackSpeed -= 1.f;
				m_bFinish = true;
			}
		}
		else
		{
			m_bSkilAdapt = false;
			m_bFinish = false;
			m_duration = 0.f;
		}

		if (KEY_TAP(KEY::S) && !m_vectorSkilCool[0].m_bSkilUse[1])
		{
			m_vectorSkilCool[0].m_bSkilUse[1] = true;
			m_vectorSkilCool[0].m_skilCoolTime[1] = 14.f;

			SetAnimDir(m_dir, L"MadAxe", true);
			CreateHitBox(Vector2(m_dir * 60.f, -50.f), Vector2(m_dir * 60.f, -50.f), Vector2(100.f, 100.f), g_physicalDamage);
		}
	}
}


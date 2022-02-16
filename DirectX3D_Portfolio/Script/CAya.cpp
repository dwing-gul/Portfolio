#include "pch.h"
#include "CAya.h"
#include "CItemManager.h"
#include "CBulletScript.h"
#include "CIGItemObjScript.h"
#include "CEffectScript.h"
#include "CHitBoxScript.h"

#include <Engine\CRenderManager.h>
#include <Engine\CAnimator3D.h>
#include <Engine\CNaviMesh.h>

CAya::CAya() :
	CPlayerScript((UINT)SCRIPT_TYPE::AYA),
	m_bPassiveOn(false),
	m_passiveCT(0.f),
	m_passiveDuration(0.f),
	m_dashDistance(500.f),
	m_rSkillDistance(450.f),
	m_Q_ReloadingTime(0.f),
	m_clipCount(6),
	m_clipMaxCount(6),
	m_dashEndPos(),
	m_dashDir(),
	m_curDashDistance(0.f),
	m_dashSpeed(500.f),
	m_shotDir(),
	m_wSkillDamage(0.f),
	m_shotCount(0),
	m_bShot(false),
	m_qSkillDamage(0.f),
	m_rSkillDuration(0.f),
	m_W_ReloadingTime(0.f),
	m_W_Distance(720.f),
	m_bWallCol(false),
	m_bullet(nullptr),
	m_qShotCount(0),
	m_skillR_Line(nullptr)
{
	m_offense = 22.f;
	m_defense = 25.f;
	m_maxHP = 665;
	m_curHP = m_maxHP;
	m_maxStamina = 440.f;
	m_curStamina = m_maxStamina;
	m_moveSpeed = 3.05f;
	m_attackSpeed = 0.16f;
	m_healthRegen = 0.5f;
	m_staminaRegen = 2.3f;
	m_criticalChance = 0.f;
	m_FOV = 8;
	m_vecWeaponType.push_back(ITEM_TYPE::PISTOL);
	m_characterType = CHARACTER_TYPE::AYA;
	SetWeaponName(ITEM_TYPE::PISTOL);
	m_arrEquipItems[0] = ITEM::WALTHER_PPK;

	m_rightHandID = 44;
	m_leftHandID = 20;

	m_attackRange = 600.f;

	m_bEquipChange = true;
}

CAya::CAya(const CAya& _origin) :
	CPlayerScript(_origin),
	m_bPassiveOn(false),
	m_passiveCT(0.f),
	m_passiveDuration(0.f),
	m_dashDistance(500.f),
	m_rSkillDistance(450.f),
	m_Q_ReloadingTime(0.f),
	m_clipCount(6),
	m_clipMaxCount(6),
	m_dashEndPos(),
	m_dashDir(),
	m_curDashDistance(0.f),
	m_dashSpeed(500.f),
	m_shotDir(),
	m_wSkillDamage(0.f),
	m_shotCount(0),
	m_bShot(false),
	m_qSkillDamage(0.f),
	m_rSkillDuration(0.f),
	m_W_ReloadingTime(0.f),
	m_W_Distance(720.f),
	m_bWallCol(false),
	m_bullet(nullptr),
	m_qShotCount(0),
	m_skillR_Line(nullptr)
{
	m_vecWeaponType.push_back(ITEM_TYPE::PISTOL);
	m_characterType = CHARACTER_TYPE::AYA;
	SetWeaponName(ITEM_TYPE::PISTOL);
	m_arrEquipItems[0] = ITEM::WALTHER_PPK;

	m_rightHandID = 44;
	m_leftHandID = 20;

	m_attackRange = 600.f;

	m_bEquipChange = true;
}

CAya::~CAya()
{
}


void CAya::start()
{
	CPlayerScript::start();

	m_attackRange = 600.f;

	vector<CGameObject*> childObj = Object()->GetChilds();

	if (!childObj.empty())
	{
		for (int i = 0; i < childObj.size(); ++i)
		{
			if (L"Weapon_Pistol" == childObj[i]->GetName())
			{
				m_weapon = childObj[i];
				CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_weapon->GetScripts()[0];
				IgItemScript->SetOwnerObj(Object());
				IgItemScript->SetEquipOption(true, m_rightHandID);
				IgItemScript->SetLocalMatrixUpdate(true);
				m_EquipItemMaterial = IgItemScript->GetCurMaterial();
				Animator3D()->SetBoneMaterial(m_EquipItemMaterial);
			}
			if (L"Aya_R_Effect" == childObj[i]->GetName())
			{
				m_skillR_Line = childObj[i];

				Ptr<CTexture> Ring_02 = CResourceManager::GetInst()->Load<CTexture>(L"Ring_02", L"texture\\FX_BI_Ring_02.png");
				CEffectScript* skill_R_effect = (CEffectScript*)m_skillR_Line->GetScriptComponent((UINT)SCRIPT_TYPE::EFFECTSCRIPT);
				skill_R_effect->SetMaterialSetting();
				skill_R_effect->SetEffectColor(Vec3(0.f, 0.850f, 0.9f));
				skill_R_effect->Reset();
				CMaterial* EffectMtrl = skill_R_effect->GetCurMaterial();
				EffectMtrl->SetData(SHADER_PARAM::TEXTURE_0, Ring_02.Get());
			}
		}
	}

	if (nullptr == m_weapon)
	{
		CPrefab* Weapon_Pistol = CResourceManager::GetInst()->Load<CPrefab>(L"Weapon_Pistol", L"prefab\\Weapon_Pistol.prefab").Get();
		m_weapon = Weapon_Pistol->Instantiate();

		CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_weapon->GetScripts()[0];
		IgItemScript->SetOwnerObj(Object());
		IgItemScript->SetEquipOption(true, m_rightHandID);
		IgItemScript->SetLocalMatrixUpdate(true);
		m_EquipItemMaterial = IgItemScript->GetCurMaterial();
		Animator3D()->SetBoneMaterial(m_EquipItemMaterial);
		CScript::AddChild(Object(), m_weapon);
	}
	
	if (nullptr == m_skillR_Line)
	{
		Ptr<CTexture> Ring_02 = CResourceManager::GetInst()->Load<CTexture>(L"Ring_02", L"texture\\FX_BI_Ring_02.png");

		CPrefab* Aya_R_Effect = CResourceManager::GetInst()->Load<CPrefab>(L"Aya_R_Effect", L"prefab\\Aya_R_Effect.prefab").Get();
		m_skillR_Line = Aya_R_Effect->Instantiate();

		m_skillR_Line->SetName(L"Aya_R_Effect");
		CScript::AddChild(Object(), m_skillR_Line);

		CEffectScript* skill_R_effect = (CEffectScript*)m_skillR_Line->GetScriptComponent((UINT)SCRIPT_TYPE::EFFECTSCRIPT);
		skill_R_effect->SetMaterialSetting();
		skill_R_effect->SetEffectColor(Vec3(0.f, 0.850f, 0.9f));
		skill_R_effect->Reset();
		skill_R_effect->GetCurMaterial()->SetData(SHADER_PARAM::TEXTURE_0, Ring_02.Get());
	}

	m_bullet = CResourceManager::GetInst()->Load<CPrefab>(L"Bullet", L"prefab\\Bullet.prefab").Get();
	
	m_weapon->SetObjOn();
	m_skillR_Line->SetObjOff();

	tBelongings belongings;
	/*belongings.ID = ITEM::OIL;
	belongings.count = 1;
	m_arrInventory[0] = belongings;*/

	belongings.ID = ITEM::BREAD;
	belongings.count = 1;
	m_arrInventory[1] = belongings;

	belongings.ID = ITEM::MEAT;
	belongings.count = 1;
	m_arrInventory[2] = belongings;

	belongings.ID = ITEM::WATER;
	belongings.count = 1;
	m_arrInventory[3] = belongings;

	//사운드
	m_skillQ_Attack01_Sound = CResourceManager::GetInst()->Load<CSound>(L"aya_Skill01_Attack_1", L"sound\\aya\\aya_Skill01_Attack_2_v1.wav");
	m_skillQ_Attack02_Sound = CResourceManager::GetInst()->Load<CSound>(L"aya_Skill01_Attack_2", L"sound\\aya\\aya_Skill01_Attack_2_v2.wav");
	m_skillW_Shot_Sound = CResourceManager::GetInst()->Load<CSound>(L"aya_Skill02_Shot", L"sound\\aya\\aya_Skill02_Shot.wav");
	m_skillE_Sound = CResourceManager::GetInst()->Load<CSound>(L"aya_Skill03_Activation", L"sound\\aya\\aya_Skill03_Activation.wav");
	m_skillR_Activation_Sound = CResourceManager::GetInst()->Load<CSound>(L"aya_Skill04_Activation", L"sound\\aya\\aya_Skill04_Activation.wav");
	m_attackSound = CResourceManager::GetInst()->Load<CSound>(L"PistolShot", L"sound\\aya\\Alex_Normal_Pistol_Fire.wav");
	
	CResourceManager::GetInst()->Load<CSound>(L"Aya_Die", L"sound\\aya\\Aya_die_1_ko.wav");
	CResourceManager::GetInst()->Load<CSound>(L"ReloadPistol", L"sound\\aya\\reloadPistol.wav");
	CResourceManager::GetInst()->Load<CSound>(L"PistolShot", L"sound\\aya\\Alex_Normal_Pistol_Fire.wav");
	CResourceManager::GetInst()->Load<CSound>(L"Aya_PlaySkill01_Voice", L"sound\\aya\\Aya_PlaySkill01_Voice.wav");
	CResourceManager::GetInst()->Load<CSound>(L"Aya_PlaySkill02_Voice", L"sound\\aya\\Aya_PlaySkill02_Voice.wav");
	CResourceManager::GetInst()->Load<CSound>(L"Aya_PlaySkill03_Voice", L"sound\\aya\\Aya_PlaySkill03_Voice.wav");
	CResourceManager::GetInst()->Load<CSound>(L"Aya_PlaySkill04_Voice", L"sound\\aya\\Aya_PlaySkill04_Voice.wav");

	AddLootingTree(ITEM::MAGNUM_BOA);
	AddLootingTree(ITEM::BUTLERS_SUIT);
	AddLootingTree(ITEM::TACTICAL_OPS_HELMET);
	AddLootingTree(ITEM::SHEATH_OF_SHAH_JAHAN);
	AddLootingTree(ITEM::WHITE_RHINOS);
	AddLootingTree(ITEM::BUCCANEER_DOUBLOON);
	AddLootingTree(ITEM::HAMBURGER);
}

void CAya::update()
{
	tItem& weapon = CItemManager::GetInst()->GetItemInfo(m_arrEquipItems[(UINT)EQUIP_TYPE::WEAPON]);

	if (m_bPassiveOn)
	{
		m_passiveDuration += DELTATIME;

		if (m_passiveDuration >= 1.5f)
		{
			m_bPassiveOn = false;
			m_passiveCT = 30.f;
			//보호막 사라지게 하기
			m_shield = 0.f;
		}
	}

	if (KEY_TAP(KEY::K) && PLAYER_TYPE::PLAYER == m_playerType)
	{
		m_arrEquipItems[(UINT)EQUIP_TYPE::WEAPON] = ITEM::MAGNUM_BOA;
		m_arrEquipItems[(UINT)EQUIP_TYPE::CHEST] = ITEM::BUTLERS_SUIT;
		m_arrEquipItems[(UINT)EQUIP_TYPE::HEAD] = ITEM::TACTICAL_OPS_HELMET;
		m_arrEquipItems[(UINT)EQUIP_TYPE::ARM] = ITEM::SHEATH_OF_SHAH_JAHAN;
		m_arrEquipItems[(UINT)EQUIP_TYPE::LEG] = ITEM::WHITE_RHINOS;
		m_arrEquipItems[(UINT)EQUIP_TYPE::ACCESSORY] = ITEM::BUCCANEER_DOUBLOON;
	}

	if (PLAYER_TYPE::PLAYER == m_playerType && !m_bMotion && !m_bReLoadingMotion && KEY_TAP(KEY::T))
	{
		m_bReLoadingMotion = true;
		m_bMotion = true;
		SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::RELOAD, false);
		//재장전 애니메이션 동작
		// 애니메이션 동작 끝났을 경우 탄창 재장전하게 제작

		m_weapon_Sound = CResourceManager::GetInst()->FindRes<CSound>(L"ReloadPistol");
		if (nullptr != m_weapon_Sound.Get() && IsPlaySound())
			m_weapon_Sound->Play(1, 0.1f);
	}
	if (m_bReLoadingMotion && Animator3D()->IsAnimFinish())
	{
		m_bReLoadingMotion = false;
		SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::WAIT, false);
	}


	if (m_bSkill_Q_Motion)
	{
		if ((Animator3D()->GetCurFrameIndex() == 5 && 0 == m_qShotCount) 
			|| (Animator3D()->GetCurFrameIndex() == 15	&& m_qShotCount == 1))
		{
			if (m_qShotCount == 0 && nullptr != m_skillQ_Attack01_Sound.Get() && IsPlaySound())
				m_skillQ_Attack01_Sound->Play(1, 0.1f);
			if (m_qShotCount == 1 && nullptr != m_skillQ_Attack02_Sound.Get() && IsPlaySound())
				m_skillQ_Attack02_Sound->Play(1, 0.1f);

			CreateBullet(m_attackTarget);

			++m_qShotCount;
		}
		if (m_qShotCount == 2 && !Animator3D()->IsAnimFinish())
		{
			m_bSkill_Q_Motion = false;
			SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::WAIT, false);
			m_qShotCount = 0;
		}
	}

	if (m_bSkill_W_Motion)
	{
		if (PLAYER_TYPE::PLAYER == m_playerType && m_shotCount != 2 && KEY_TAP(KEY::W))
		{
			m_bSkill_W_Motion = false;
			SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::WAIT, false);
			m_bShot = false;
			m_skill_W_CT = 20.f - (m_skill_W_Lv * 2.f);
			m_skill_W_MaxCT = m_skill_W_CT;
			if (nullptr != m_skillW_Shot_Sound.Get())
				m_skillW_Shot_Sound->Stop();
		}

		if (m_bShot)
		{
			if (Animator3D()->GetCurFrameIndex() == 1)
			{
				m_shotDir = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
				m_bShot = false;
				CreateBullet();
				SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::SKILL02_SHOT, false);

				if (nullptr != m_skillW_Shot_Sound.Get() && IsPlaySound())
					m_skillW_Shot_Sound->Play(1, 0.1f, true);

				++m_shotCount;
			}		
		}
		else
		{
			if (Animator3D()->IsAnimFinish() && Animator3D()->GetCurAnimName() == L"Aya_Pistol_skill02_shot")
			{
				m_bShot = true;
				Animator3D()->Reset();

				if (m_shotCount >= 10)
				{
					m_shotCount = 0;
					m_bSkill_W_Motion = false;
					SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::WAIT, false);
					m_skill_W_CT = 20.f - (m_skill_W_Lv * 2.f);
					m_skill_W_MaxCT = m_skill_W_CT;
				}
			}
		}
	}

	if (m_bSkill_E_Motion)
	{
		Vec3 pos = Transform()->GetWorldPos();
		pos -= m_dashDir * m_dashSpeed * DELTATIME;
		m_curDashDistance = m_dashSpeed * DELTATIME;

		//모션 애니메이션이 끝난 경우 추가
		wstring Name = Animator3D()->GetFindClipName(m_curWeaponName + L"_skill03");
		wstring CurName = Animator3D()->GetCurAnimName();
		if (Animator3D()->IsAnimFinish() || m_curDashDistance >= m_dashDistance)
		{
			m_bSkill_E_Motion = false;
			SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::WAIT, false);
		}
		if (m_naviMesh->NaviMesh()->IsAboveTile(Object(), -m_dashDir)
			&& 20 >= Animator3D()->GetCurFrameIndex())		//통과 불가능한 벽과 충돌한 경우
			Transform()->SetLocalPos(pos);
	}

	if (m_bSkill_R_Motion)
	{
		m_rSkillDuration += DELTATIME;

		if (Animator3D()->GetCurFrameIndex() == 36 && m_attackBox != nullptr && !m_attackBox->IsOn())
		{
			m_attackBox->SetOnOff(true);
			//어택 박스에게 대미지 및 속박 지급해야함
			//거리에 따라 대미지 및 속박 시간이 다름
			//속박 지속시간은 0.5초~ 1.5/1.75/2초 , 레벨당

			float damage = (150.f * (float)m_skill_R_Lv) + m_finalAttackPower;
			float extraSkillDamage = m_ExtraSkillDamage + (m_ExtraSkillDamagePerLevel * m_level);
			float skillTime = 1.25f + (m_skill_R_Lv * 0.25f);
			float skillAmp = m_SkillAmplification + (m_SkillAmplificationPerLevel * m_level);
			m_attackBox->Collider3D()->SetOffsetPos(Vec3(0.f, -6.f, 0.f));
			m_attackBox->Collider3D()->SetOffsetScale(Vec3(6.3f, 6.3f, 6.3f));
			m_attackBox->Collider3D()->SetSphere(true);
			((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDamage(damage);
			((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraPercent(extraSkillDamage);
			((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraFlat(skillAmp);
			((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDebuff(DEBUFF_TYPE::NONMOVE);
			((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDebuffTime(skillTime);

			if (nullptr != m_skillR_Activation_Sound.Get() && IsPlaySound())
				m_skillR_Activation_Sound->Play(1, 0.1f);
		}
		if (Animator3D()->GetCurFrameIndex() == 42 && m_attackBox != nullptr && m_attackBox->IsOn())
		{
			m_attackBox->SetObjOff();

			if (nullptr != m_skillR_Line)
			{
				CEffectScript* skill_R_effect = (CEffectScript*)m_skillR_Line->GetScriptComponent((UINT)SCRIPT_TYPE::EFFECTSCRIPT);
				skill_R_effect->SetAlphaOff();
			}
		}

		if (Animator3D()->IsAnimFinish() || m_rSkillDuration >= 1.5f)
		{
			m_bSkill_R_Motion = false;
			SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::WAIT, false);
			m_skill_R_CT = 115.f - ((float)m_skill_R_Lv * 25.f);
			m_skill_R_MaxCT = m_skill_R_CT;

		}
	}

	if (nullptr != m_skillR_Line && !m_skillR_Line->IsOn() && nullptr != m_attackBox && m_attackBox->IsOn())
	{
		m_attackBox->SetObjOff();
	}

	Attack();

	CPlayerScript::update();
}

void CAya::lateUpdate()
{
	CPlayerScript::lateUpdate();
	
	if (m_passiveCT > 0.f)
		m_passiveCT -= DELTATIME;
}

void CAya::OnColliderEnter(CCollider3D* _other)
{
}

void CAya::OnCollider(CCollider3D* _other)
{
}

void CAya::OnColliderExit(CCollider3D* _other)
{
}

void CAya::SetAnimAction(PLAYER_ANIMTYPE _action, UINT _AnimType, bool _bLoop, float _animSpeed, UINT _count)
{
	if (PLAYER_ANIMTYPE::END == _action)
		return;


	if (m_animType == _action && m_curAnim == _AnimType)
	{
		if (_bLoop != Animator3D()->GetRepeat())
			Animator3D()->SetRepeat(_bLoop, _count);
		return;
	}

	if (PLAYER_ANIMTYPE::COMMON_ANIM == _action)
		CPlayerScript::SetAnimAction(_action, _AnimType, _bLoop, _animSpeed, _count);
	else
	{
		wstring animName = m_curWeaponName + L"_";

		switch ((AYA_ANIM)_AnimType)
		{
		case AYA_ANIM::RUN:
			animName += L"run";
			break;
		case AYA_ANIM::WAIT:
			animName += L"wait";
			break;
		case AYA_ANIM::ATK01:
			animName += L"atk01";
			break;
		case AYA_ANIM::ATK02:
			animName += L"atk02";
			break;
		case AYA_ANIM::SKILL01:
			animName += L"skill01";
			break;
		case AYA_ANIM::SKILL02_RUN_BACK:
			animName += L"skill02_run_back";
			break;
		case AYA_ANIM::SKILL02_RUN_FORWARD:
			animName += L"skill02_run_forward";
			break;
		case AYA_ANIM::SKILL02_RUN_LEFT:
			animName += L"skill02_run_left";
			break;
		case AYA_ANIM::SKILL02_RUN_RIGHT:
			animName += L"skill02_run_right";
			break;
		case AYA_ANIM::SKILL02_WAIT:
			animName += L"skill02_wait";
			break;
		case AYA_ANIM::SKILL02_SHOT:
			animName += L"skill02_shot";
			break;
		case AYA_ANIM::SKILL03:
			animName += L"skill03";
			break;
		case AYA_ANIM::SKILL04:
			animName += L"skill04";
			break;
		case AYA_ANIM::SKILL04_START:
			animName += L"Skill04_Start";
			break;
		case AYA_ANIM::SKILL04_END:
			animName += L"Skill04_End";
			break;
		case AYA_ANIM::WEAPONSKILL_STAR:
			animName += L"Weaponskill_start";
			break;
		case AYA_ANIM::WEAPONSKILL_LOOP:
			animName += L"Weaponskill_loop";
			break;
		case AYA_ANIM::WEAPONSKILL_SHOT:
			animName += L"Weaponskill_shot";
			break;
		case AYA_ANIM::WEAPONSKILL_END:
			animName += L"Weaponskill_end";
			break;
		case AYA_ANIM::RELOAD:
			animName += L"reload";
			break;			
		case AYA_ANIM::END:
			return;
		default:
			return;
		}

		Animator3D()->Play(animName, _bLoop, _animSpeed, _count);
		m_animType = _action;
		m_curAnim = _AnimType;
	}
}

void CAya::StatsReSet()
{
	CPlayerScript::StatsReSet();

	m_offense = 22.f;
	m_defense = 25.f;
	m_maxHP = 665;
	m_curHP = m_maxHP;
	m_maxStamina = 440.f;
	m_curStamina = m_maxStamina;
	m_moveSpeed = 3.05f;
	m_attackSpeed = 0.16f;
	m_healthRegen = 0.5f;
	m_staminaRegen = 2.3f;
	m_criticalChance = 0.f;
	m_FOV = 8;
}

void CAya::LvUp()
{
	CPlayerScript::LvUp();

	m_offense += 3.0f;
	m_defense += 2.5f;
	m_maxHP += 56.f;
	m_curHP += 56.f;
	m_maxStamina += 25.f;
	m_curStamina += 25.f;
	m_healthRegen += 0.04f;
	m_staminaRegen += 0.07f;
}

void CAya::Attack()
{
	// 타깃 지정시 타깃의 거리가 공격 범위 내라면 데미지를 준다.
	if (nullptr == m_attackTarget)
		return;

	if (((CObjectScript*)m_attackTarget->GetScripts()[0])->IsCharacterDead() && m_attackTarget->GetObjectType() == OBJECT_TYPE::MONSTER)
	{
		return;
	}

	if (PLAYER_TYPE::PLAYER == m_playerType && !m_bGoToAttack)
	{
		return;
	}

	Vec3 targetPos = m_attackTarget->Transform()->GetWorldPos_XZ();
	Vec3 pos = Transform()->GetWorldPos_XZ();
	float dist = Vec3::Distance(pos, targetPos);

	// 아야 5 , 5
	if (nullptr != m_attackTarget && m_bAttack && !m_bTargetAttack)
	{
		if (Animator3D()->GetCurFrameIndex() == 5 && 
			(m_curAnim == (UINT)AYA_ANIM::ATK01 || m_curAnim == (UINT)AYA_ANIM::ATK02))
		{
			m_bTargetAttack = true;
			CreateBullet(m_attackTarget);

			m_attackSound = CResourceManager::GetInst()->FindRes<CSound>(L"PistolShot");
			if(nullptr != m_attackSound.Get() && IsPlaySound())
				m_attackSound->Play(1, 0.1f);

			// 기본 공격시 패시브 쿨타임 1초 감소
			if (m_passiveCT > 0.f)
				m_passiveCT -= 1.f;
		}
	}

	Vec3 rot = Transform()->GetLocalRot();
	Vec3 front = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	front.Normalize();
	Vec3 dir = (targetPos - pos).Normalize();

	if (m_bAttack)
	{
		m_rotAngle = 0.f;

		Vec3 up = Transform()->GetWorldDir(DIR_TYPE::UP);
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

		float yAngle = rot.y;

		rot.y += m_rotAngle;

		if (_isnanf(rot.y))
			rot.y = yAngle;

		Transform()->SetLocalRot(rot);
	}

	if (dist < m_attackRange && !m_bMotion)	//스킬 사용중 아닐대, 어택 모션일때 
	{
		m_bAttack = true;
		m_bAttackDistance = true;
	}
	else if(dist > m_attackRange && !m_naviMesh->NaviMesh()->IsMove())
	{
		m_bAttackDistance = false;
	}
}

void CAya::Hit(ATTACK_TYPE _type, float _damage, float _critical, float _extraPercent, float _extraFlat, CGameObject* _owner)
{
	// 맞았는지 확인 및 체력 깎음
	m_bHit = true;

	if (PLAYER_TYPE::AI == m_playerType && nullptr != _owner && _owner->GetObjectType() == OBJECT_TYPE::PLAYER)
		m_attackTarget = _owner;

	float def = m_finalDefense * (1 - m_defReduce);
	float baseDamage = _damage * 100 / (100 + def);
	float extraDamage_percent;
	float extraDamage_flat;
	float finalBaseDamage = (baseDamage * _critical);
	float finalDamage = 0.f;

	// 계산 다시 해야됨
	if (ATTACK_TYPE::BASE == _type)
	{
		extraDamage_percent = 1 + _extraPercent;
		extraDamage_flat = _extraFlat - m_DefenseFromBasicAttack;
	}
	else
	{
		extraDamage_percent = 1 + _extraPercent - m_SkillDamageReduction_Percent;
		extraDamage_flat = _extraFlat - m_SkillDamageReduction;
	}

	if (1.f < _critical)
	{
		finalBaseDamage *= (1 - m_CriticalDamageReduction);
	}

	finalDamage = finalBaseDamage * extraDamage_percent + extraDamage_flat;

	//해당 스킬은 쿨타임 아이템 영향을 받지 않음
	if (m_passiveCT <= 0.f)
	{
		m_bPassiveOn = true;
		m_passiveDuration = 0.f;
		m_shield = (125.f + (float)m_passive_LV * 25.f) + m_offense * 0.3f;
	}
	
	if (m_bPassiveOn)
	{
		if (m_shield < finalDamage)
		{
			finalDamage -= m_shield;
		}
		else
		{
			m_shield -= finalDamage;
			finalDamage -= m_shield;

			if (finalDamage < 0.f)
			{
				finalDamage = 0.f;
			}
		}
	}
	else
	{
		if (finalDamage < 1.f)
		{
			finalDamage = 1.f;
		}
	}

	m_curHP -= finalDamage;

	CObjectScript::Hit(_type, _damage, _critical, _extraPercent, _extraFlat, _owner);

	if (m_bCharacterDead && nullptr != _owner && _owner->GetObjectType() == OBJECT_TYPE::PLAYER)
	{
		((CPlayerScript*)_owner->GetScripts()[0])->AddEXP();
	}
}


void CAya::Skill_Q(CGameObject* _target)
{
	if (_target == nullptr)
		return;

	m_skill_Q_CT = 7.5f - (0.5f * m_skill_Q_Lv);
	m_curStamina -= m_skill_Q_SM;

	m_skill_Q_MaxCT = m_skill_Q_CT;
	m_bSkill_Q_Motion = true;

	m_bMoveMotion = false;
	SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::SKILL01, false);
	m_qSkillDamage = m_finalAttackPower + (35.f + (float)(m_skill_Q_Lv - 1) * 55.f);

	m_skillVoiceSound = CResourceManager::GetInst()->FindRes<CSound>(L"Aya_PlaySkill01_Voice");
	if (nullptr != m_skillVoiceSound.Get() && IsPlaySound())
		m_skillVoiceSound->Play(1, 0.1f);
}

void CAya::Skill_W(CGameObject* pObj)
{
	m_bSkill_W_Motion = true;
	m_bShot = true;
	m_curStamina -= m_skill_W_SM;

	m_bMoveMotion = false;

	m_W_ReloadingTime = 0.f;
	m_wSkillDamage = m_finalAttackPower + (35.f + (float)(m_skill_W_Lv - 1) * 55.f);
	CreateBullet();

	SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::SKILL02_WAIT, false);
	//사거리 7.2m
	//스킬 지속시간 3초
	//만약 연속으로 공격을 맞은 상대에게 대미지 -5% 지급해야함

	m_skillVoiceSound = CResourceManager::GetInst()->FindRes<CSound>(L"Aya_PlaySkill02_Voice");
	if (nullptr != m_skillVoiceSound.Get() && IsPlaySound())
		m_skillVoiceSound->Play(1, 0.1f);
}

void CAya::Skill_E(CGameObject* pObj)
{
	m_curStamina -= m_skill_E_SM;
	m_bSkill_E_Motion = true;

	
	m_dashDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	m_dashEndPos = Transform()->GetWorldPos();
	m_dashEndPos -= m_dashDir * m_dashDistance;
	m_curDashDistance = 0.f;
	m_skill_E_CT = 22.f - (m_skill_E_Lv * 2.f);
	m_skill_E_MaxCT = m_skill_E_CT;

	//대쉬 거리4m
	//해당 애니메이션 동작
	SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::SKILL03, false);

	if (nullptr != m_skillE_Sound.Get() && IsPlaySound())
		m_skillE_Sound->Play(1, 0.1f);

	m_skillVoiceSound = CResourceManager::GetInst()->FindRes<CSound>(L"Aya_PlaySkill03_Voice");
	if (nullptr != m_skillVoiceSound.Get() && IsPlaySound())
		m_skillVoiceSound->Play(1, 0.1f);
}

void CAya::Skill_R(CGameObject* obj)
{
	m_skill_R_SM = 100.f;
	m_curStamina -= m_skill_R_SM;
	m_bSkill_R_Motion = true;

	m_bMoveMotion = false;

	m_rSkillDuration = 0.f;
	//해당 애니메이션 동작
	//사거리 4.5m
	SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)AYA_ANIM::SKILL04, false);

	if (nullptr != m_skillR_Line)
	{
		m_skillR_Line->SetObjOn();
		CEffectScript* skill_R_effect = (CEffectScript*)m_skillR_Line->GetScriptComponent((UINT)SCRIPT_TYPE::EFFECTSCRIPT);
		skill_R_effect->Reset();
	}

	m_skillVoiceSound = CResourceManager::GetInst()->FindRes<CSound>(L"Aya_PlaySkill04_Voice");
	if (nullptr != m_skillVoiceSound.Get() && IsPlaySound())
		m_skillVoiceSound->Play(1, 0.1f);
}

void CAya::CreateBullet(CGameObject* _target)
{
	if (nullptr == m_bullet)
		return;

	CGameObject* bullet = m_bullet->Instantiate();
	bullet->Transform()->SetLocalPos(Transform()->GetWorldPos() + Vec3(0.f, 120.f, 0.f));
	Vec3 rot = bullet->Transform()->GetLocalRot();
	bullet->Transform()->SetLocalRot(Transform()->GetLocalRot() + rot);

	srand((unsigned int)time(NULL));
	int criticalPercent = rand() % 100;
	bool bCritical = false;

	if (criticalPercent < int(m_finalCriticalChance * 100.f))
		bCritical = true;

	if (bCritical)
	{
		((CBulletScript*)bullet->GetScripts()[0])->SetCritical(m_criticalDamage);
	}
	else
	{
		((CBulletScript*)bullet->GetScripts()[0])->SetCritical(1.f);
	}

	((CBulletScript*)bullet->GetScripts()[0])->SetOwner(Object());
	((CBulletScript*)bullet->GetScripts()[0])->SetSpeed(600.f);

	if (m_bSkill_W_Motion)
	{
		//W 스킬일때
		// 7.2m
		((CBulletScript*)bullet->GetScripts()[0])->SetDir(m_shotDir);
		((CBulletScript*)bullet->GetScripts()[0])->SetDamage(m_wSkillDamage);
		((CBulletScript*)bullet->GetScripts()[0])->SetExtraPercent(m_SkillAmplification + (m_SkillAmplificationPerLevel * m_level));
		((CBulletScript*)bullet->GetScripts()[0])->SetExtraFlat(m_ExtraSkillDamage + (m_ExtraSkillDamagePerLevel * m_level));
		CScript::CreateObject(bullet, bullet->Transform()->GetLocalPos(), Object()->GetLayerIndex());
	}
	else
	{
		//Q 스킬일때 기본 공격과 동일하게 타겟팅 스킬이어서 기본 공격 처럼 적용할 예정
		// 기본공격과 사거리 동일

		if (nullptr == _target)
			return;

		((CBulletScript*)bullet->GetScripts()[0])->SetTarget(_target);
		
		if (m_bSkill_Q_Motion) // q 스킬 사용
		{
			((CBulletScript*)bullet->GetScripts()[0])->SetDamage(m_qSkillDamage);
			((CBulletScript*)bullet->GetScripts()[0])->SetAttackType(ATTACK_TYPE::SKILL);
			((CBulletScript*)bullet->GetScripts()[0])->SetExtraPercent(m_SkillAmplification + (m_SkillAmplificationPerLevel * m_level));
			((CBulletScript*)bullet->GetScripts()[0])->SetExtraFlat(m_ExtraSkillDamage + (m_ExtraSkillDamagePerLevel * m_level));
		}
		else // 기본 공격
		{
			((CBulletScript*)bullet->GetScripts()[0])->SetDamage(m_finalAttackPower);
			((CBulletScript*)bullet->GetScripts()[0])->SetExtraPercent(0.f);
			((CBulletScript*)bullet->GetScripts()[0])->SetExtraFlat(m_ExtraBaseAttackDamage + (m_ExtraBaseAttackDamagePerLevel * m_level));
		}

		CScript::CreateObject(bullet, bullet->Transform()->GetLocalPos(), Object()->GetLayerIndex());
	}
	
}

void CAya::SaveToScene(FILE* _file)
{
	CPlayerScript::SaveToScene(_file);
}

void CAya::LoadFromScene(FILE* _file)
{
	CPlayerScript::LoadFromScene(_file);
}

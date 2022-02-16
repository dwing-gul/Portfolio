#include "pch.h"
#include "CLiDailin.h"
#include "CEffectScript.h"
#include "CIGItemObjScript.h"
#include "CHitBoxScript.h"

#include <Engine\CRenderManager.h>
#include <Engine\CAnimator3D.h>
#include <Engine\CNaviMesh.h>
#include "CMouseRay.h"

CLiDailin::CLiDailin():
	CPlayerScript((UINT)SCRIPT_TYPE::LIDAILIN),
	m_wSkill_Duration(0.f),
	m_qSkill_Stack(0),
	m_skillDistance(0.f),
	m_curSkillDistance(0.f),
	m_skillStartPos(),
	m_skillEndPos(),
	m_skillDir(),
	m_skillDamage(0.f),
	m_bObjCollision(false),
	m_bWallCol(false),
	m_rSkill_Stack(0),
	m_skillQ_Effect(nullptr),
	m_skillW_Effect(nullptr),
	m_skillE_Effect(nullptr)
{
	m_offense = 31.f;
	m_defense = 22.f;
	m_maxHP = 740.f;
	m_curHP = m_maxHP;
	m_maxStamina = 420.f;
	m_curStamina = m_maxStamina;
	m_moveSpeed = 3.1f;
	m_attackSpeed = 0.10f;
	m_healthRegen = 1.1f;
	m_staminaRegen = 0.2f;
	m_criticalChance = 0.f;
	m_FOV = 8;
	m_attackRange = 195.f;
	m_vecWeaponType.push_back(ITEM_TYPE::GLOVES);
	SetWeaponName(ITEM_TYPE::GLOVES);
	m_characterType = CHARACTER_TYPE::LIDAILIN;
	m_curWeaponName = L"Glove";
	m_rightHandID = 80;
	m_leftHandID = 20;

	m_arrEquipItems[0] = ITEM::COTTON_GLOVES;

	m_bEquipChange = true;
}

CLiDailin::CLiDailin(const CLiDailin& _origin) :
	CPlayerScript(_origin),
	m_wSkill_Duration(0.f),
	m_qSkill_Stack(0),
	m_skillDistance(0.f),
	m_curSkillDistance(0.f),
	m_skillStartPos(),
	m_skillEndPos(),
	m_skillDir(),
	m_skillDamage(0.f),
	m_bObjCollision(false),
	m_bWallCol(false),
	m_rSkill_Stack(0),
	m_skillQ_Effect(nullptr),
	m_skillW_Effect(nullptr),
	m_skillE_Effect(nullptr)
{
	m_vecWeaponType.push_back(ITEM_TYPE::GLOVES);
	SetWeaponName(ITEM_TYPE::GLOVES);
	m_characterType = CHARACTER_TYPE::LIDAILIN;
	m_curWeaponName = L"Glove";
	m_rightHandID = 80;
	m_leftHandID = 20;
	m_arrEquipItems[0] = ITEM::COTTON_GLOVES;

	m_bEquipChange = true;
}

CLiDailin::~CLiDailin()
{
}

void CLiDailin::start()
{
	CPlayerScript::start();

	//리다이린 왼손 20뼈, 오른손 80
	vector<CGameObject*> childObj = Object()->GetChilds();

	if (!childObj.empty())
	{
		Ptr<CTexture> effect;
		Ptr<CMaterial> material;
		Ptr<CMesh> mesh;
		CEffectScript* effectScript;

		CIGItemObjScript* IgItemScript = nullptr;
		for (int i = 0; i < childObj.size(); ++i)
		{
			if (L"LIDailin_Q_Effect" == childObj[i]->GetName())
			{
				m_skillQ_Effect = childObj[i];
				effect = CResourceManager::GetInst()->Load<CTexture>(L"hyunwoo_q_Effect", L"texture\\hyunwoo_q_Effect.png");
				CEffectScript* effectScript = (CEffectScript*)m_skillQ_Effect->GetScripts()[0];

				effectScript->SetMaterialSetting();
				material = effectScript->GetCurMaterial();
				material->SetData(SHADER_PARAM::TEXTURE_0, effect.Get());
				effectScript->SetDecalEffect(true);
			}
			if (L"LIDailin_Bottle" == childObj[i]->GetName())
			{
				m_bottle = childObj[i];
				IgItemScript = (CIGItemObjScript*)m_bottle->GetScripts()[0];
				IgItemScript->SetOwnerObj(Object());
				IgItemScript->SetEquipOption(true, m_rightHandID);
				IgItemScript->SetLocalMatrixUpdate(true);
			}
			if (L"Range_Dailin" == childObj[i]->GetName())
			{
				m_skillE_Effect = childObj[i];
			}
			if (L"LIDailin_W_Effect" == childObj[i]->GetName())
			{
				m_skillW_Effect = childObj[i];
				m_skillW_Effect->Transform()->SetLocalPos(Vec3(0.f, 2.5f, 0.f));
				effect = CResourceManager::GetInst()->Load<CTexture>(L"FX_BI_Dailin", L"texture\\FX_BI_Dailin_01.png");

				effectScript = (CEffectScript*)m_skillW_Effect->GetScripts()[0];

				effectScript->SetMaterialSetting();
				material = effectScript->GetCurMaterial();
				effectScript->SetEffectColor(Vec3(1.f, 0.f, 0.f));
				material->SetData(SHADER_PARAM::TEXTURE_0, effect.Get());
				effectScript->SetBuffEffectType();
				effectScript->SetMode(1);
			}
		}
	}

	
	if (nullptr == m_skillQ_Effect || nullptr == m_bottle
		|| nullptr == m_skillE_Effect || nullptr == m_skillW_Effect)
	{
		Ptr<CTexture>	effect;
		Ptr<CMaterial>	material;
		CEffectScript* effectScript;

		if (nullptr == m_skillQ_Effect)
		{
			// 리다이린 Q 이펙트
			effect = CResourceManager::GetInst()->Load<CTexture>(L"hyunwoo_q_Effect", L"texture\\hyunwoo_q_Effect.png");
			CPrefab* Q_Effect = CResourceManager::GetInst()->Load<CPrefab>(L"LIDailin_Q_Effect", L"prefab\\LIDailin_Q_Effect.prefab").Get();
			m_skillQ_Effect = Q_Effect->Instantiate();
			m_skillQ_Effect->SetName(L"LIDailin_Q_Effect");
			//내려찍는 타이밍때 생성시키기
			CEffectScript* effectScript = (CEffectScript*)m_skillQ_Effect->GetScripts()[0];
			effectScript->SetMaterialSetting();
			material = effectScript->GetCurMaterial();
			material->SetData(SHADER_PARAM::TEXTURE_0, effect.Get());
			effectScript->SetDecalEffect(true);

			CScript::AddChild(Object(), m_skillQ_Effect);
		}

		if (nullptr == m_bottle)
		{
			//W 이펙트 , 생성되는 위치를 플레이어 머리 위로 설정하면 됨
			CPrefab* LIDailin_Bottle = CResourceManager::GetInst()->Load<CPrefab>(L"LIDailin_Bottle", L"prefab\\LIDailin_Bottle.prefab").Get();
			m_bottle = LIDailin_Bottle->Instantiate();

			m_bottle->SetName(L"LIDailin_Bottle");
			CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_bottle->GetScripts()[0];
			IgItemScript->SetOwnerObj(Object());
			IgItemScript->SetEquipOption(true, m_rightHandID);
			IgItemScript->SetLocalMatrixUpdate(true);

			m_EquipItemMaterial = IgItemScript->GetCurMaterial();
			Animator3D()->SetBoneMaterial(m_EquipItemMaterial);
			CScript::AddChild(Object(), m_bottle);
		}

		if (nullptr == m_skillE_Effect)
		{
			CPrefab* Range_Dailin_01 = CResourceManager::GetInst()->Load<CPrefab>(L"Range_Dailin", L"prefab\\Range_Dailin.prefab").Get();
			m_skillE_Effect = Range_Dailin_01->Instantiate();
			CScript::AddChild(Object(), m_skillE_Effect);
		}

		if (nullptr == m_skillW_Effect)
		{
			CPrefab* W_Effect = CResourceManager::GetInst()->Load<CPrefab>(L"LIDailin_W_Effect", L"prefab\\LIDailin_W_Effect.prefab").Get();
			m_skillW_Effect = W_Effect->Instantiate();
			m_skillW_Effect->Transform()->SetLocalPos(Vec3(0.f, 2.5f, 0.f));
			effect = CResourceManager::GetInst()->Load<CTexture>(L"FX_BI_Dailin", L"texture\\FX_BI_Dailin_01.png");
			m_skillW_Effect->SetName(L"LIDailin_W_Effect");

			CScript::AddChild(Object(), m_skillW_Effect);
			effectScript = (CEffectScript*)m_skillW_Effect->GetScripts()[0];
			effectScript->SetMaterialSetting();
			effectScript->SetEffectColor(Vec3(1.f, 0.f, 0.f));
			material = effectScript->GetCurMaterial();
			material->SetData(SHADER_PARAM::TEXTURE_0, effect.Get());
			effectScript->SetBuffEffectType();
			effectScript->SetMode(1);
		}
	}

	//사운드 로드
	m_skillQ_1_Sound	= CResourceManager::GetInst()->Load<CSound>(L"Lidailin_Skill01_Attack01", L"sound\\LiDailin\\Lidailin_Skill01_Attack01.wav");
	m_skillQ_2_Sound	= CResourceManager::GetInst()->Load<CSound>(L"Lidailin_Skill01_Attack02", L"sound\\LiDailin\\Lidailin_Skill01_Attack02.wav");
	m_skillQ_3_Sound	= CResourceManager::GetInst()->Load<CSound>(L"Lidailin_Skill01_Attack03", L"sound\\LiDailin\\Lidailin_Skill01_Attack03_1.wav");
	m_skillW_Sound		= CResourceManager::GetInst()->Load<CSound>(L"Lidailin_Skill02_Active", L"sound\\LiDailin\\Lidailin_Skill02_Active.wav");
	m_skillE_Sound		= CResourceManager::GetInst()->Load<CSound>(L"Lidailin_Skill03_Attack", L"sound\\LiDailin\\Lidailin_Skill03_Attack.wav");
	m_skillR_Dash_Sound = CResourceManager::GetInst()->Load<CSound>(L"Lidailin_Skill04_Dash", L"sound\\LiDailin\\Lidailin_Skill04_Dash.wav");
	m_attackSound = CResourceManager::GetInst()->Load<CSound>(L"Glove_Hit", L"sound\\character\\hitGlove_in_r1.wav");
	
	CResourceManager::GetInst()->Load<CSound>(L"LiDailin_Die", L"sound\\LiDailin\\LiDailin_die_1_ko.wav");
	CResourceManager::GetInst()->Load<CSound>(L"LiDailin_PlaySkillVoice_Q_01", L"sound\\LiDailin\\LiDailin_PlaySkillVoice_Q_01.wav");
	CResourceManager::GetInst()->Load<CSound>(L"LiDailin_PlaySkillVoice_Q_02", L"sound\\LiDailin\\LiDailin_PlaySkillVoice_Q_02.wav");
	CResourceManager::GetInst()->Load<CSound>(L"LiDailin_PlaySkillVoice_Q_03", L"sound\\LiDailin\\LiDailin_PlaySkillVoice_Q_03.wav");
	CResourceManager::GetInst()->Load<CSound>(L"LiDailin_PlaySkillVoice_W", L"sound\\LiDailin\\LiDailin_PlaySkillVoice_W.wav");
	CResourceManager::GetInst()->Load<CSound>(L"LiDailin_PlaySkillVoice_E", L"sound\\LiDailin\\LiDailin_PlaySkillVoice_E.wav");
	CResourceManager::GetInst()->Load<CSound>(L"LiDailin_PlaySkillVoice_R", L"sound\\LiDailin\\LiDailin_PlaySkillVoice_R.wav");
	
	CResourceManager::GetInst()->Load<CSound>(L"Lidailin_Skill04_Hit01", L"sound\\LiDailin\\Lidailin_Skill04_Hit_p.wav");
	CResourceManager::GetInst()->Load<CSound>(L"Lidailin_Skill04_Hit02", L"sound\\LiDailin\\Lidailin_Skill04_Hit_p_02.wav");
	CResourceManager::GetInst()->Load<CSound>(L"Lidailin_Skill04_Hit03", L"sound\\LiDailin\\Lidailin_Skill04_Hit_p_03.wav");
	CResourceManager::GetInst()->Load<CSound>(L"Lidailin_Skill04_Hit04", L"sound\\LiDailin\\Lidailin_Skill04_Hit_p_04.wav");

	m_bottle->SetObjOff();
	m_skillW_Effect->SetObjOff();
	m_skillE_Effect->SetObjOff();

	AddLootingTree(ITEM::FROST_PETAL_HAND);
	AddLootingTree(ITEM::KABANA);
	AddLootingTree(ITEM::IMPERIAL_BURGONET);
	AddLootingTree(ITEM::SHEATH_OF_SHAH_JAHAN);
	AddLootingTree(ITEM::TACHYON_BRACE);
	AddLootingTree(ITEM::WHITE_CRANE_FAN);
}

void CLiDailin::update()
{
	CPlayerScript::update();

	if (KEY_TAP(KEY::K) && PLAYER_TYPE::PLAYER == m_playerType)
	{
		m_arrEquipItems[(UINT)EQUIP_TYPE::WEAPON] = ITEM::FROST_PETAL_HAND;
		m_arrEquipItems[(UINT)EQUIP_TYPE::CHEST] = ITEM::KABANA;
		m_arrEquipItems[(UINT)EQUIP_TYPE::HEAD] = ITEM::SHEATH_OF_SHAH_JAHAN;
		m_arrEquipItems[(UINT)EQUIP_TYPE::ARM] = ITEM::IMPERIAL_BURGONET;
		m_arrEquipItems[(UINT)EQUIP_TYPE::LEG] = ITEM::TACHYON_BRACE;
		m_arrEquipItems[(UINT)EQUIP_TYPE::ACCESSORY] = ITEM::WHITE_CRANE_FAN;
	}

	if (m_bSkill_Q_Motion)
	{
		Vec3 pos = Transform()->GetWorldPos();
		pos.x += (300.f * m_skillDir * DELTATIME).x;
		pos.z += (300.f * m_skillDir * DELTATIME).z;

		m_curSkillDistance += 300.f * DELTATIME;

		if (nullptr != m_attackBox)
		{
			if (m_qSkill_Stack == 1)
			{
				if (!m_attackBox->IsOn() && Animator3D()->GetCurFrameIndex() == 1)
				{
					//어택 박스 생성
					m_attackBox->SetOnOff(true);

					// 어택 박스 충돌 시 데미지 부여
					float extraSkillDamage = m_ExtraSkillDamage + (m_ExtraSkillDamagePerLevel * m_level);
					float skillAmp = m_SkillAmplification + (m_SkillAmplificationPerLevel * m_level);
					m_attackBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
					m_attackBox->Collider3D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
					m_attackBox->Collider3D()->SetSphere(true);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDamage(m_skillDamage);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraPercent(extraSkillDamage);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraFlat(skillAmp);
				}
				else if (m_attackBox->IsOn() && Animator3D()->GetCurFrameIndex() == 10)
				{
					m_attackBox->SetObjOff();
				}
				
			}
			else if (m_qSkill_Stack == 2)
			{
				if (!m_attackBox->IsOn() && Animator3D()->GetCurFrameIndex() == 3)
				{
					//어택 박스 생성
					m_attackBox->SetOnOff(true);

					// 어택 박스 충돌 시 데미지 부여
					float extraSkillDamage = m_ExtraSkillDamage + (m_ExtraSkillDamagePerLevel * m_level);
					float skillAmp = m_SkillAmplification + (m_SkillAmplificationPerLevel * m_level);
					m_attackBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
					m_attackBox->Collider3D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
					m_attackBox->Collider3D()->SetSphere(true);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDamage(m_skillDamage);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraPercent(extraSkillDamage);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraFlat(skillAmp);
				}
				else if (m_attackBox->IsOn() && Animator3D()->GetCurFrameIndex() == 12)
				{
					m_attackBox->SetObjOff();
				}
			}
			else if (m_qSkill_Stack == 0)
			{
				if (!m_attackBox->IsOn() && Animator3D()->GetCurFrameIndex() == 8)
				{
					//어택 박스 생성
					m_attackBox->SetOnOff(true);

					// 어택 박스 충돌 시 데미지 부여
					float extraSkillDamage = m_ExtraSkillDamage + (m_ExtraSkillDamagePerLevel * m_level);
					float skillAmp = m_SkillAmplification + (m_SkillAmplificationPerLevel * m_level);
					m_attackBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
					m_attackBox->Collider3D()->SetOffsetScale(Vec3(2.f, 2.f, 2.f));
					m_attackBox->Collider3D()->SetSphere(true);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDamage(m_skillDamage);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraPercent(extraSkillDamage);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraFlat(skillAmp);


					if (nullptr != m_skillQ_Effect)
					{
						CEffectScript* effectScript = (CEffectScript*)m_skillQ_Effect->GetScripts()[0];
						effectScript->Reset();
						m_skillQ_Effect->SetObjOn();

						if (nullptr != m_skillQ_3_Sound.Get() && IsPlaySound())
						{
							m_skillQ_3_Sound->Play(1, 0.1f);
						}
						m_skillVoiceSound = CResourceManager::GetInst()->FindRes<CSound>(L"LiDailin_PlaySkillVoice_Q_03");
						if (nullptr != m_skillVoiceSound.Get() && IsPlaySound())
							m_skillVoiceSound->Play(1, 0.1f);
					}
				}
				else if (m_attackBox->IsOn() && Animator3D()->GetCurFrameIndex() == 15)
				{
					m_attackBox->SetObjOff();
				}
			}
		}
	
		if (m_curSkillDistance >= m_skillDistance || Animator3D()->IsAnimFinish())
		{
			SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::WAIT, false);
			m_bSkill_Q_Motion = false;
		}
		if (m_naviMesh->NaviMesh()->IsAboveTile(Object(), m_skillDir))		//통과 불가능한 벽과 충돌한 경우
			Transform()->SetLocalPos(pos);
	}
	
	if (m_bSkill_W_Motion)
	{
		if (Animator3D()->IsAnimFinish())
		{
			m_skillVoiceSound = CResourceManager::GetInst()->FindRes<CSound>(L"LiDailin_PlaySkillVoice_W");
			if (nullptr != m_skillVoiceSound.Get() && IsPlaySound())
			{
				m_skillVoiceSound->Play(1, 0.1f);
			}

			if (nullptr != m_bottle)
			{
				m_bottle->SetObjOff();
			}
			m_bSkill_W_Motion = false;
			SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::WAIT, false);
		}
			
	}
	
	if (m_bSkill_E_Motion)
	{
		if (nullptr != m_skillE_Effect)
		{
			if (Animator3D()->GetCurFrameIndex() == 8 && !m_skillE_Effect->IsOn())
			{
				m_skillE_Effect->SetObjOn();
				if(nullptr != m_skillE_Sound.Get() && IsPlaySound())
					m_skillE_Sound->Play(1, 0.1f);

				m_skillVoiceSound = CResourceManager::GetInst()->FindRes<CSound>(L"LiDailin_PlaySkillVoice_E");
				if(nullptr != m_skillVoiceSound.Get() && IsPlaySound())
					m_skillVoiceSound->Play(1, 0.1f);


				// 상대방 20% 슬로우
				m_skillDamage = 25.f + (55.f * m_skill_E_Lv) + (m_finalAttackPower * 0.5f);

				if (nullptr != m_attackBox && !m_attackBox->IsOn())
				{
					//어택 박스 생성
					m_attackBox->SetOnOff(true);

					// 어택 박스 충돌 시 데미지 부여
					float extraSkillDamage = m_ExtraSkillDamage + (m_ExtraSkillDamagePerLevel * m_level);
					float skillAmp = m_SkillAmplification + (m_SkillAmplificationPerLevel * m_level);
					m_attackBox->Collider3D()->SetOffsetPos(Vec3(0.f, 80.f, -120.f));
					m_attackBox->Collider3D()->SetOffsetScale(Vec3(1.5f, 1.f, 2.f));
					m_attackBox->Collider3D()->SetSphere(false);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDamage(m_skillDamage);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraPercent(extraSkillDamage);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraFlat(skillAmp);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDebuff(DEBUFF_TYPE::SLOW);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDebuffTime(3.f);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDebuffRate(0.2f);
				}
				
			}

			if (Animator3D()->GetCurFrameIndex() == 28 && m_skillE_Effect->IsOn())
			{
				m_skillE_Effect->SetObjOff();

				if (nullptr != m_attackBox && m_attackBox->IsOn())
				{
					m_attackBox->SetObjOff();
				}
			}
		}

		if (Animator3D()->IsAnimFinish())
		{
			SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::WAIT, false);
			m_bSkill_E_Motion = false;
		}
	}

	if (m_bSkill_R_Motion)
	{
		if (m_bObjCollision)
		{
			if (m_rSkill_Stack != 4)
			{
				if (Animator3D()->GetCurFrameIndex() == 1 && m_rSkill_Stack == 0)
				{
					m_skillR_Attack_Sound = CResourceManager::GetInst()->FindRes<CSound>(L"Lidailin_Skill04_Hit01");
					if(nullptr != m_skillR_Attack_Sound.Get() && IsPlaySound())
						m_skillR_Attack_Sound->Play(1, 0.1f);

					Skill_R_Attack();
					++m_rSkill_Stack;
				}
				if (Animator3D()->GetCurFrameIndex() == 10 && m_rSkill_Stack == 1)
				{
					m_skillR_Attack_Sound = CResourceManager::GetInst()->FindRes<CSound>(L"Lidailin_Skill04_Hit02");
					if (nullptr != m_skillR_Attack_Sound.Get() && IsPlaySound())
						m_skillR_Attack_Sound->Play(1, 0.1f);

					Skill_R_Attack();
					++m_rSkill_Stack;
				}
				if (Animator3D()->GetCurFrameIndex() == 24 && m_rSkill_Stack == 2)
				{
					m_skillR_Attack_Sound = CResourceManager::GetInst()->FindRes<CSound>(L"Lidailin_Skill04_Hit03");
					if (nullptr != m_skillR_Attack_Sound.Get() && IsPlaySound())
						m_skillR_Attack_Sound->Play(1, 0.1f);

					Skill_R_Attack();
					++m_rSkill_Stack;
				}
				if (Animator3D()->GetCurFrameIndex() == 32 && m_rSkill_Stack == 3)
				{
					m_skillR_Attack_Sound = CResourceManager::GetInst()->FindRes<CSound>(L"Lidailin_Skill04_Hit04");
					if (nullptr != m_skillR_Attack_Sound.Get() && IsPlaySound())
						m_skillR_Attack_Sound->Play(1, 0.1f);

					Skill_R_Attack();
					++m_rSkill_Stack;
				}
			}
			
			if (Animator3D()->IsAnimFinish())
			{
				m_bSkill_R_Motion = false;
				m_bNonStop = false;
				m_rSkill_Stack = 0; 
				m_bObjCollision = false;

				if (m_skillR_Target->GetObjectType() == OBJECT_TYPE::PLAYER)
				{
					((CPlayerScript*)m_skillR_Target->GetScripts()[0])->SetNonMove(false, 0.f);
				}
				else if (m_skillR_Target->GetObjectType() == OBJECT_TYPE::MONSTER)
				{
					((CMonsterScript*)m_skillR_Target->GetScripts()[0])->SetNonMove(false, 0.f);
				}

				m_skillR_Target = nullptr;
			}
		}
		else
		{
			Vec3 pos = Transform()->GetWorldPos();
			pos += (800.f * -m_skillDir * DELTATIME);

			m_curSkillDistance += 800.f * DELTATIME;
			if (m_curSkillDistance >= m_skillDistance || Animator3D()->IsAnimFinish())
			{
				SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::WAIT, false);
				m_bSkill_R_Motion = false;
				m_bNonStop = false;
			}
			if (m_naviMesh->NaviMesh()->IsAboveTile(Object(), m_skillDir))		//통과 불가능한 벽과 충돌한 경우
				Transform()->SetLocalPos(pos);

			// 어택 박스 충돌 시 데미지 부여
			if (nullptr != m_attackBox && ((CHitBoxScript*)m_attackBox->GetScripts()[0])->IsCharacterFirstCol())
			{
				m_skillVoiceSound = CResourceManager::GetInst()->FindRes<CSound>(L"LiDailin_PlaySkillVoice_R");
				if (nullptr != m_skillVoiceSound.Get() && IsPlaySound())
					m_skillVoiceSound->Play(1, 0.1f);

				m_skillR_Target = ((CHitBoxScript*)m_attackBox->GetScripts()[0])->GetFristTarGetObj();
				m_attackBox->SetObjOff();
				m_bObjCollision = true;
				SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::SKILL04_ATTACK_P, false);
			}
	
			m_bNonStop = true;
		}
	}
	
	Attack();
	

}

void CLiDailin::lateUpdate()
{
	CPlayerScript::lateUpdate();
}

void CLiDailin::OnColliderEnter(CCollider3D* _other)
{
}

void CLiDailin::OnCollider(CCollider3D* _other)
{
}

void CLiDailin::OnColliderExit(CCollider3D* _other)
{
}

void CLiDailin::SetAnimAction(PLAYER_ANIMTYPE _action, UINT _AnimType, bool _bLoop, float _animSpeed, UINT _count)
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

		switch ((LIDAILIN_ANIM)_AnimType)
		{
		case LIDAILIN_ANIM::RUN:
			animName += L"Run";
			break;
		case LIDAILIN_ANIM::WAIT:
			animName += L"Wait";
			break;
		case LIDAILIN_ANIM::ATK01:
			animName += L"Atk01";
			break;
		case LIDAILIN_ANIM::ATK01_P:
			animName += L"Atk01_p";
			break;
		case LIDAILIN_ANIM::ATK02:
			animName += L"Atk02";
			break;
		case LIDAILIN_ANIM::ATK02_P:
			animName += L"Atk02_p";
			break;
		case LIDAILIN_ANIM::SKILL01_01:
			animName += L"Skill01_01";
			break;
		case LIDAILIN_ANIM::SKILL01_02:
			animName += L"Skill01_02";
			break;
		case LIDAILIN_ANIM::SKILL01_03:
			animName += L"Skill01_03";
			break;
		case LIDAILIN_ANIM::SKILL01_01_P:
			animName += L"Skill01_01_p";
			break;
		case LIDAILIN_ANIM::SKILL01_02_P:
			animName += L"Skill01_02_p";
			break;
		case LIDAILIN_ANIM::SKILL01_03_P:
			animName += L"Skill01_03_p";
			break;
		case LIDAILIN_ANIM::SKILL02:
			animName += L"Skill02";
			break;
		case LIDAILIN_ANIM::RUN_SKILL02:
			animName += L"Run_Skill02";
			break;
		case LIDAILIN_ANIM::WAIT_SKILL02:
			animName += L"Wait_Skill02";
			break;
		case LIDAILIN_ANIM::SKILL03:
			animName += L"Skill03";
			break;
		case LIDAILIN_ANIM::SKILL03_P:
			animName += L"Skill03_P";
			break;
		case LIDAILIN_ANIM::SKILL04_START:
			animName += L"Skill04_Start";
			break;
		case LIDAILIN_ANIM::SKILL04_START_P:
			animName += L"Skill04_Start_P";
			break;
		case LIDAILIN_ANIM::SKILL04_ATTACK:
			animName += L"Skill04_Attack";
			break;
		case LIDAILIN_ANIM::SKILL04_ATTACK_P:
			animName += L"Skill04_Attack_p";
			break;
		case LIDAILIN_ANIM::END:
			return;
		default:
			return;
		}

		Animator3D()->Play(animName, _bLoop, _animSpeed, _count);
		m_animType = _action;
		m_curAnim = _AnimType;
	}
}

void CLiDailin::StatsReSet()
{
	CPlayerScript::StatsReSet();

	m_offense = 31.f;
	m_defense = 22.f;
	m_maxHP = 740.f;
	m_curHP = m_maxHP;
	m_maxStamina = 420.f;
	m_curStamina = m_maxStamina;
	m_moveSpeed = 3.1f;
	m_attackSpeed = 0.10f;
	m_healthRegen = 1.1f;
	m_staminaRegen = 0.2f;
	m_criticalChance = 0.f;
}

void CLiDailin::LvUp()
{
	CPlayerScript::LvUp();

	m_offense += 2.5f;
	m_defense += 2.2f;
	m_maxHP += 92.f;
	m_curHP += 92.f;
	m_maxStamina += 16.f;
	m_curStamina += 16.f;
	m_healthRegen += 0.07f;
	m_staminaRegen += 0.01f;
}

void CLiDailin::SkillDirAndPosResult(float distance)
{
	m_skillDir = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
	m_skillStartPos = Transform()->GetWorldPos();
	m_skillEndPos = m_skillStartPos + m_skillDir * m_skillDistance;
}

void CLiDailin::Skill_Q(CGameObject* pObj)
{
	if (m_bSkill_Q_Motion)
		return;

	m_skillDamage = 5.f + (20.f * m_skill_Q_Lv) + (m_finalAttackPower * 0.35f);
	m_skillDistance = 200.f;
	SkillDirAndPosResult(m_skillDistance);

	switch (m_qSkill_Stack)
	{
	case 0:
	{	
		++m_qSkill_Stack;

		if (nullptr != m_skillQ_1_Sound && IsPlaySound())
		{
			m_skillQ_1_Sound->Play(1, 0.1f);

			m_skillVoiceSound = CResourceManager::GetInst()->FindRes<CSound>(L"LiDailin_PlaySkillVoice_Q_01");
			if (nullptr != m_skillVoiceSound.Get() && IsPlaySound())
				m_skillVoiceSound->Play(1, 0.1f);
		}
		
		SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::SKILL01_01, false);
	}
	break;
	case 1:
	{
		++m_qSkill_Stack;
		if (nullptr != m_skillQ_2_Sound && IsPlaySound())
		{
			m_skillQ_2_Sound->Play(1, 0.1f);

			m_skillVoiceSound = CResourceManager::GetInst()->FindRes<CSound>(L"LiDailin_PlaySkillVoice_Q_02");
			if (nullptr != m_skillVoiceSound.Get() && IsPlaySound())
				m_skillVoiceSound->Play(1, 0.1f);
		}
		SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::SKILL01_02, false);
	}
	break;
	case 2:
	{
		SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::SKILL01_03, false);

		m_qSkill_Stack = 0;
		m_skill_Q_CT = (12.f * (1 - m_CooldownReductionCap)) * (1 - m_CooldownReduction);
		
		m_skill_Q_MaxCT = m_skill_Q_CT;
	}
	break;
	default:
		return;
	}

	m_curSkillDistance = 0.f;
	m_bSkill_Q_Motion = true;
}

void CLiDailin::Skill_W(CGameObject* pObj)
{
	m_skill_W_CT = ((12.5f * (1 - m_CooldownReductionCap)) - (m_skill_W_Lv * 1.5f)) * (1 - m_CooldownReduction);

	m_skill_W_MaxCT = m_skill_W_CT;
	m_bSkill_W_Motion = true;
	//0.8초 동안 애니메이션 진행
	//해당 시간 동안 공격 회피
	m_wSkill_Duration = 0.8f;
	SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::SKILL02, false , 1.f);

	if (nullptr != m_skillW_Sound.Get() && IsPlaySound())
	{
		m_skillW_Sound->Play(1, 0.1f);
	}

	//해당 이펙트도 리다이린 술마시는 모션일 때 생성
	if (nullptr != m_skillW_Effect)
	{
		m_skillW_Effect->SetObjOn();
		CEffectScript* effectScript = (CEffectScript*)m_skillW_Effect->GetScripts()[0];
		effectScript->Reset();
	}
	if (nullptr != m_bottle)
	{
		m_bottle->SetObjOn();
		CIGItemObjScript* IgItemScript = (CIGItemObjScript*)m_bottle->GetScripts()[0];
		m_EquipItemMaterial = IgItemScript->GetCurMaterial();
		Animator3D()->SetBoneMaterial(m_EquipItemMaterial);
	}
}

void CLiDailin::Skill_E(CGameObject* pObj)
{
	m_curStamina -= m_skill_E_SM;
	m_skill_E_CT = ((14.f * (1.f - m_CooldownReductionCap)) - float(m_skill_E_Lv)) * (1.f - m_CooldownReduction);

	m_skill_E_MaxCT = m_skill_E_CT;

	if (nullptr != m_skillE_Sound.Get() && IsPlaySound())
	{
		m_skillE_Sound->Play(1, 0.1f);
	}
			
	m_skillDir = -Transform()->GetWorldDir(DIR_TYPE::FRONT);

	SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::SKILL03, false);
	m_bSkill_E_Motion = true;
}

void CLiDailin::Skill_R(CGameObject* pObj)
{
	m_curStamina -= m_skill_R_SM;
	m_skill_R_CT = ((220.f * (1.f - m_CooldownReductionCap)) - (m_skill_R_Lv * 35.f)) * (1.f - m_CooldownReduction);
	
	m_skill_R_MaxCT = m_skill_R_CT;

	m_skillDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	m_skillStartPos = Transform()->GetWorldPos();
	//사거리 5.5m
	m_skillDistance = 550.f;
	m_skillEndPos = m_skillStartPos - m_skillDir * m_skillDistance;

	m_curSkillDistance = 0.f;
	m_bSkill_R_Motion = true;
	m_skillDamage = -15.f + (90.f * m_skill_R_Lv) + (m_finalAttackPower * 0.5f);

	SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)LIDAILIN_ANIM::SKILL04_START, false);

	if (nullptr != m_skillR_Dash_Sound.Get() && IsPlaySound())
		m_skillR_Dash_Sound->Play(1, 0.1f);

	//어택 박스 생성
	m_attackBox->SetOnOff(true);

	float extraSkillDamage = m_ExtraSkillDamage + (m_ExtraSkillDamagePerLevel * m_level);
	float skillAmp = m_SkillAmplification + (m_SkillAmplificationPerLevel * m_level);
	m_attackBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
	m_attackBox->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
	m_attackBox->Collider3D()->SetSphere(false);
	((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDamage(m_skillDamage);
	((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraPercent(extraSkillDamage);
	((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraFlat(skillAmp);
	((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDebuff(DEBUFF_TYPE::NONMOVE);
}

void CLiDailin::Skill_R_Attack()
{
	// 어택 박스 충돌 시 데미지 부여
	float extraSkillDamage = m_ExtraSkillDamage + (m_ExtraSkillDamagePerLevel * m_level);
	float skillAmp = m_SkillAmplification + (m_SkillAmplificationPerLevel * m_level);
	srand((unsigned int)time(NULL));
	int criticalPercent = rand() % 100;
	bool bCritical = false;

	if (criticalPercent < int(m_finalCriticalChance * 100.f))
		bCritical = true;

	if (m_skillR_Target->GetObjectType() == OBJECT_TYPE::MONSTER)
	{
		if (!((CMonsterScript*)m_skillR_Target->GetScripts()[0])->IsHit())
		{
			if (bCritical)
			{
				((CMonsterScript*)m_skillR_Target->GetScripts()[0])->Hit(ATTACK_TYPE::SKILL, m_skillDamage, m_criticalDamage, skillAmp, extraSkillDamage, Object());
			}
			else
			{
				((CMonsterScript*)m_skillR_Target->GetScripts()[0])->Hit(ATTACK_TYPE::SKILL, m_skillDamage, 1.f, skillAmp, extraSkillDamage, Object());
			}
		}
	}
	else if (Object() != m_skillR_Target && m_skillR_Target->GetObjectType() == OBJECT_TYPE::PLAYER)
	{
		if (!((CPlayerScript*)m_skillR_Target->GetScripts()[0])->IsHit())
		{
			if (bCritical)
			{
				((CPlayerScript*)m_skillR_Target->GetScripts()[0])->Hit(ATTACK_TYPE::SKILL, m_skillDamage, m_criticalDamage, skillAmp, extraSkillDamage, Object());
			}
			else
			{
				((CPlayerScript*)m_skillR_Target->GetScripts()[0])->Hit(ATTACK_TYPE::SKILL, m_skillDamage, 1.f, skillAmp, extraSkillDamage, Object());
			}
		}
	}
}

void CLiDailin::SaveToScene(FILE* _file)
{
	CPlayerScript::SaveToScene(_file);
}

void CLiDailin::LoadFromScene(FILE* _file)
{
	CPlayerScript::LoadFromScene(_file);
}

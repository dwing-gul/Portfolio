#include "pch.h"
#include "CHyunwooScript.h"
#include "CMonsterScript.h"
#include "CEffectScript.h"
#include "CHitBoxScript.h"

#include <Engine\CNaviMesh.h>
#include <Engine\CRenderManager.h>
#include <Engine\CAnimator3D.h>

CHyunwooScript::CHyunwooScript() :
	CPlayerScript((UINT)SCRIPT_TYPE::HYUNWOOSCRIPT),
	m_passiveStack(0),
	m_passiveMaxStack(9),
	m_bObjCollision(false),
	m_bWallCollsion(false),
	m_assaultStratPos(),
	m_assaultDir(),
	m_assaultEndPos(),
	m_assaultDistance(500.f),
	m_bAssaultMotion(false),
	m_bCharge(false),
	m_assaultSpeed(800.f),
	m_bBuff(false),
	m_chargeTime(0.f),
	m_chargeRadio(0.f),
	m_curAssaultDistance(0.f),
	m_skill_W_CumTime(0.f),
	m_chargeMaxTime(3.f),
	m_bAssaultEndMotion(false),
	m_bAssaultLoopMotion(false),
	m_bAssaultStartMotion(false),
	m_bChargeStartMotion(false),
	m_skill_R_RangeLine(nullptr),
	m_skill_R_Range(nullptr),
	m_skillQ_Effect(nullptr),
	m_skillW_Effect(nullptr),
	m_bPassiveStackUp(false)
{
	m_offense = 38.f;
	m_defense = 23.f;
	m_maxHP = 670.f;
	m_curHP = m_maxHP;
	m_maxStamina = 350.f;
	m_curStamina = m_maxStamina;
	m_moveSpeed = 3.45f;
	m_attackSpeed = 0.14f;
	m_healthRegen = 0.8f;
	m_staminaRegen = 1.8f;
	m_criticalChance = 0.f;
	m_FOV = 8.f;			//시야의 경우 인 게임에서 확인 후 지정
	m_attackRange = 195.f;
	m_vecWeaponType.push_back(ITEM_TYPE::GLOVES);
	m_characterType = CHARACTER_TYPE::HYUNWOO;
	SetWeaponName(ITEM_TYPE::GLOVES);
	m_arrEquipItems[0] = ITEM::COTTON_GLOVES;

	m_rightHandID = 45;
	m_leftHandID = 22;

	m_bEquipChange = true;
}

CHyunwooScript::CHyunwooScript(const CHyunwooScript& _origin) :
	CPlayerScript(_origin),
	m_passiveStack(0),
	m_passiveMaxStack(_origin.m_passiveMaxStack),
	m_bObjCollision(false),
	m_bWallCollsion(false),
	m_assaultStratPos(),
	m_assaultDir(),
	m_assaultEndPos(),
	m_assaultDistance(500.f),
	m_bAssaultMotion(false),
	m_bCharge(false),
	m_assaultSpeed(800.f),
	m_bBuff(false),
	m_chargeTime(0.f),
	m_chargeRadio(0.f),
	m_curAssaultDistance(0.f),
	m_skill_W_CumTime(0.f),
	m_chargeMaxTime(3.f),
	m_bAssaultEndMotion(false),
	m_bAssaultLoopMotion(false),
	m_bAssaultStartMotion(false),
	m_bChargeStartMotion(false),
	m_skill_R_RangeLine(nullptr),
	m_skill_R_Range(nullptr),
	m_skillQ_Effect(nullptr),
	m_skillW_Effect(nullptr),
	m_bPassiveStackUp(false)
{
	m_vecWeaponType.push_back(ITEM_TYPE::GLOVES);
	m_characterType = CHARACTER_TYPE::HYUNWOO;
	SetWeaponName(ITEM_TYPE::GLOVES);
	m_arrEquipItems[0] = ITEM::COTTON_GLOVES;

	m_rightHandID = 45;
	m_leftHandID = 22;

	m_bEquipChange = true;
}

CHyunwooScript::~CHyunwooScript()
{
}

void CHyunwooScript::start()
{
	CPlayerScript::start();

	vector<CGameObject*> childObj = Object()->GetChilds();

	if (!childObj.empty())
	{
		Ptr<CTexture> effectText;
		Ptr<CMaterial> material;
		Ptr<CMesh> mesh;
		CEffectScript* effectScript;

		for (int i = 0; i < childObj.size(); ++i)
		{
			if (L"hyunwoo_Q_Effect" == childObj[i]->GetName())
			{
				m_skillQ_Effect = childObj[i];
				effectText = CResourceManager::GetInst()->Load<CTexture>(L"hyunwoo_q_Effect", L"texture\\hyunwoo_q_Effect.png");
				effectScript = (CEffectScript*)m_skillQ_Effect->GetScripts()[0];
				effectScript->SetMaterialSetting();
				material = effectScript->GetCurMaterial();
				material->SetData(SHADER_PARAM::TEXTURE_0, effectText.Get());
				effectScript->SetEffectColor(Vec3(1.f, 1.0f, 1.0f));
				effectScript->SetDecalEffect(true);
			}
			if (L"hyunwoo_R_RangeLine" == childObj[i]->GetName())
			{
				m_skill_R_RangeLine = childObj[i];
			}
			if (L"hyunwoo_R_Range" == childObj[i]->GetName())
			{
				m_skill_R_Range = childObj[i];
			}
			if (L"hyunwoo_W_Effect" == childObj[i]->GetName())
			{
				m_skillW_Effect = childObj[i];
				m_skillW_Effect->Transform()->SetLocalPos(Vec3(0.f, 2.5f, 0.f));
				effectText = CResourceManager::GetInst()->Load<CTexture>(L"FX_BI_Shield_01", L"texture\\FX_BI_Shield_01.png");

				effectScript = (CEffectScript*)m_skillW_Effect->GetScripts()[0];
				effectScript->SetMaterialSetting();
				material = effectScript->GetCurMaterial();
				material->SetData(SHADER_PARAM::TEXTURE_0, effectText.Get());
				effectScript->SetBuffEffectType();
			}
		}
	}
	Ptr<CTexture>	effectText;
	Ptr<CMaterial>	material;
	Ptr<CMesh>		mesh;

	CEffectScript* effectScript;
	// 이펙트 생성하기
	// Q 스킬
	if (nullptr == m_skillQ_Effect)
	{
		effectText = CResourceManager::GetInst()->Load<CTexture>(L"hyunwoo_Q_Effect", L"texture\\hyunwoo_q_Effect.png");

		CPrefab* hyunwoo_Q_Effect = CResourceManager::GetInst()->Load<CPrefab>(L"hyunwoo_Q_Effect", L"prefab\\hyunwoo_Q_Effect.prefab").Get();
		m_skillQ_Effect = hyunwoo_Q_Effect->Instantiate();
		m_skillQ_Effect->SetName(L"hyunwoo_Q_Effect");
		effectScript = (CEffectScript*)m_skillQ_Effect->GetScripts()[0];
		effectScript->SetMaterialSetting();
		effectScript->GetCurMaterial()->SetData(SHADER_PARAM::TEXTURE_0, effectText.Get());
		effectScript->SetEffectColor(Vec3(1.f, 1.0f, 1.0f));
		effectScript->SetDecalEffect(true);

		CScript::AddChild(Object(), m_skillQ_Effect);
	}
	
	if (nullptr == m_skillW_Effect)
	{
		effectText = CResourceManager::GetInst()->Load<CTexture>(L"FX_BI_Shield_01", L"texture\\FX_BI_Shield_01.png");
		// W 스킬
		CPrefab* hyunwoo_W_Effect = CResourceManager::GetInst()->Load<CPrefab>(L"hyunwoo_W_Effect", L"prefab\\hyunwoo_W_Effect.prefab").Get();
		m_skillW_Effect = hyunwoo_W_Effect->Instantiate();
		m_skillW_Effect->SetName(L"hyunwoo_W_Effect");
		m_skillW_Effect->Transform()->SetLocalPos(Vec3(0.f, 2.5f, 0.f));
		CScript::AddChild(Object(), m_skillW_Effect);
		effectScript = (CEffectScript*)m_skillW_Effect->GetScripts()[0];
		effectScript->SetMaterialSetting();
		effectScript->GetCurMaterial()->SetData(SHADER_PARAM::TEXTURE_0, effectText.Get());
		effectScript->SetBuffEffectType();
	}

	//R 스킬 라인
	if (nullptr == m_skill_R_RangeLine)
	{
		CPrefab* hyunwoo_R_RangeLine = CResourceManager::GetInst()->Load<CPrefab>(L"hyunwoo_R_RangeLine", L"prefab\\hyunwoo_R_RangeLine.prefab").Get();
		m_skill_R_RangeLine = hyunwoo_R_RangeLine->Instantiate();
		m_skill_R_RangeLine->SetName(L"hyunwoo_R_RangeLine");
		material = CResourceManager::GetInst()->FindRes<CMaterial>(L"Effect3DDefaultMaterial").Get();
		material = material->Clone();
		m_skill_R_RangeLine->MeshRender()->SetMaterial(material);

		Vec3 color = Vec3(1.f, 0.1f, 0.1f);
		float ratio = 1.f;
		material->SetData(SHADER_PARAM::VEC4_0, &color);
		material->SetData(SHADER_PARAM::FLOAT_0, &ratio);
		CScript::AddChild(Object(), m_skill_R_RangeLine);
	}
	
	if (nullptr == m_skill_R_Range)
	{
		CPrefab* hyunwoo_R_Range = CResourceManager::GetInst()->Load<CPrefab>(L"hyunwoo_R_Range", L"prefab\\hyunwoo_R_Range.prefab").Get();
		m_skill_R_Range = hyunwoo_R_Range->Instantiate();
		//R 스킬 Range
		m_skill_R_Range->SetName(L"hyunwoo_R_Range");

		float ratio = 0.f;
		material = CResourceManager::GetInst()->FindRes<CMaterial>(L"Effect3DDefaultMaterial").Get();
		material = material->Clone();
		m_skill_R_Range->MeshRender()->SetMaterial(material);
		Vec3 color = Vec3(1.f, 0.1f, 0.1f);
		material->SetData(SHADER_PARAM::VEC4_0, &color);
		material->SetData(SHADER_PARAM::FLOAT_0, &ratio);
		CScript::AddChild(Object(), m_skill_R_Range);
	}

	m_skillQ_Effect->SetObjOff();
	m_skillW_Effect->SetObjOff();
	m_skill_R_RangeLine->SetObjOff();
	m_skill_R_Range->SetObjOff();
	m_skillW_Effect->SetObjOff();

	//사운드
	m_skillQ_Attack_Sound = CResourceManager::GetInst()->Load<CSound>(L"Hyunwoo_Skill01", L"sound\\hyunwoo\\hyunwoo_Skill01_Attack_v1.wav");
	m_skillQ_Hit_Sound = CResourceManager::GetInst()->Load<CSound>(L"Hyunwoo_Skill01", L"sound\\hyunwoo\\hyunwoo_Skill01_Hit.wav");
	m_skillW_Sound = CResourceManager::GetInst()->Load<CSound>(L"Hyunwoo_Skill02", L"sound\\hyunwoo\\hyunwoo_Skill02_Activation.wav");
	m_skillE_Hit_Sound = CResourceManager::GetInst()->Load<CSound>(L"Hyunwoo_Skill03", L"sound\\hyunwoo\\hyunwoo_Skill03_Hit.wav");
	m_skillE_Slide_Sound = CResourceManager::GetInst()->Load<CSound>(L"Hyunwoo_Skill03", L"sound\\hyunwoo\\hyunwoo_Skill03_Slide.wav");
	m_skillR_Charging_Sound = CResourceManager::GetInst()->Load<CSound>(L"Hyunwoo_Skill04", L"sound\\hyunwoo\\hyunwoo_Skill04_Charging.wav");
	m_skillR_Attack_Sound = CResourceManager::GetInst()->Load<CSound>(L"Hyunwoo_Skill04", L"sound\\hyunwoo\\hyunwoo_Skill04_Attack.wav");
	m_skillR_Hit_Sound = CResourceManager::GetInst()->Load<CSound>(L"Hyunwoo_Skill04", L"sound\\hyunwoo\\hyunwoo_Skill04_Hit.wav");
	m_attackSound = CResourceManager::GetInst()->Load<CSound>(L"Glove_Hit", L"sound\\character\\hitGlove_in_r1.wav");
	
	CResourceManager::GetInst()->Load<CSound>(L"Hyunwoo_Die", L"sound\\hyunwoo\\Hyunwoo_die_2_ja.wav");
	CResourceManager::GetInst()->Load<CSound>(L"Hyunwoo_PlaySkillVoice_Q", L"sound\\hyunwoo\\Hyunwoo_PlaySkillVoice_Q.wav");
	CResourceManager::GetInst()->Load<CSound>(L"Hyunwoo_PlaySkillVoice_R", L"sound\\hyunwoo\\Hyunwoo_PlaySkillVoice_R.wav");

	AddLootingTree(ITEM::DIVINE_FIST);
	AddLootingTree(ITEM::AMAZONESS_ARMOR);
	AddLootingTree(ITEM::SWORD_STOPPER);
	AddLootingTree(ITEM::HELM_OF_BANNERET);
	AddLootingTree(ITEM::FEATHER_BOOTS);
	AddLootingTree(ITEM::MAGAZINE);
}

void CHyunwooScript::update()
{
	CPlayerScript::update();

	if (KEY_TAP(KEY::K) && PLAYER_TYPE::PLAYER == m_playerType)
	{
		m_arrEquipItems[(UINT)EQUIP_TYPE::WEAPON] = ITEM::DIVINE_FIST;
		m_arrEquipItems[(UINT)EQUIP_TYPE::CHEST] = ITEM::AMAZONESS_ARMOR;
		m_arrEquipItems[(UINT)EQUIP_TYPE::HEAD] = ITEM::HELM_OF_BANNERET;
		m_arrEquipItems[(UINT)EQUIP_TYPE::ARM] = ITEM::SWORD_STOPPER;
		m_arrEquipItems[(UINT)EQUIP_TYPE::LEG] = ITEM::FEATHER_BOOTS;
		m_arrEquipItems[(UINT)EQUIP_TYPE::ACCESSORY] = ITEM::MAGAZINE;
	}

	if (m_bBuff)
	{
		m_skill_W_CumTime += DELTATIME;

		if (m_skill_W_CumTime >= 2.5f)
		{
			m_skill_W_CumTime = 0.f;
			m_bBuff = false;
		}
	}

	if (m_bSkill_R_Motion)
	{
		if (m_bCharge)
		{
			Vec3 pos = Transform()->GetWorldPos();
		
			if (m_bChargeStartMotion)
			{
				if (Animator3D()->IsAnimFinish())
				{
					m_bChargeStartMotion = false;
					SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::SKILL04_LOOP, true);
				}
			}
			m_chargeTime += DELTATIME;

			if (PLAYER_TYPE::PLAYER == m_playerType && (KEY_NONE(KEY::R) || m_chargeMaxTime <= m_chargeTime))
			{
				m_skill_R_CT = 70.f - ((float)m_skill_R_Lv * 5.f);

				m_skill_R_MaxCT = m_skill_R_CT;
				m_bCharge = false;

				if (nullptr != m_skillR_Charging_Sound.Get())
					m_skillR_Charging_Sound->Stop();

				SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::SKILL04_END, false);

				if (nullptr != m_skillR_Attack_Sound.Get() && IsPlaySound())
					m_skillR_Attack_Sound->Play(1, 0.1f);

				m_skillVoiceSound = CResourceManager::GetInst()->FindRes<CSound>(L"Hyunwoo_PlaySkillVoice_R");
				if (nullptr != m_skillVoiceSound.Get() && IsPlaySound())
					m_skillVoiceSound->Play(1, 0.1f);

				if (nullptr != m_skill_R_RangeLine)
					m_skill_R_RangeLine->SetObjOff();

				if (nullptr != m_skill_R_Range)
					m_skill_R_Range->SetObjOff();
			}
			else
			{
				if (nullptr != m_skill_R_Range)
				{
					m_chargeRadio = m_chargeTime / m_chargeMaxTime;
					m_skill_R_Range->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_chargeRadio);
				}
			}

			//땅을 내려치는 애니메이션으로 변경,
			//어택 박스에서 대미지 입히게 함
			// 사거리 최소 3m ~ 4.2m , 차징 시간에 따라 거리 정해주기
		}
		else
		{
			if (!Animator3D()->IsAnimFinish() && nullptr != m_attackBox)
			{
				if (Animator3D()->GetCurFrameIndex() == 3 && !m_attackBox->IsOn())
				{
					//어택 박스 생성
					m_attackBox->SetObjOn();

					// 어택 박스 충돌 시 데미지 부여
					float damage = (100.f + (m_skill_R_Lv - 1) * 50.f) * ((500.f + (m_skill_R_Lv - 1) * 250.f) * (m_chargeTime / m_chargeMaxTime)) + (m_finalAttackPower * (0.6f + 1.2f * (m_chargeTime / m_chargeMaxTime)));
					float extraSkillDamage = m_ExtraSkillDamage + (m_ExtraSkillDamagePerLevel * m_level);
					float skillAmp = m_SkillAmplification + (m_SkillAmplificationPerLevel * m_level);
					m_attackBox->Collider3D()->SetOffsetPos(Vec3(0.f, 0.f, -250.f));
					m_attackBox->Collider3D()->SetOffsetScale(Vec3(3.f, 1.f, 4.f));
					m_attackBox->Collider3D()->SetSphere(false);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDamage(damage);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraPercent(extraSkillDamage);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraFlat(skillAmp);
					((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetHitSound(m_skillR_Hit_Sound.Get());
				}
				else if (Animator3D()->GetCurFrameIndex() == 15 && m_attackBox->IsOn())
				{
					m_attackBox->SetObjOff();
				}
			}
			if (Animator3D()->IsAnimFinish())
			{
				m_bSkill_R_Motion = false;
				SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::WAIT, false);
			}
		}
	}

	if (m_bSkill_Q_Motion)
	{
		if (!m_skillQ_Effect->IsOn() && Animator3D()->GetCurFrameIndex() == 10)
		{
			m_skillQ_Effect->SetObjOn();
			CEffectScript* effect = (CEffectScript*)m_skillQ_Effect->GetScripts()[0];
			effect->Reset();

			if (nullptr != m_attackBox && !m_attackBox->IsOn())
			{
				//어택 박스 생성
				m_attackBox->SetObjOn();
				//공격력에 따라 대미지를 무조건 올림 사용
				float damage = 50.f * (float)m_skill_Q_Lv + ceil((m_finalAttackPower * 0.3f));

				// 어택 박스 충돌 시 데미지 부여
				float extraSkillDamage = m_ExtraSkillDamage + (m_ExtraSkillDamagePerLevel * m_level);
				float skillAmp = m_SkillAmplification + (m_SkillAmplificationPerLevel * m_level);
				m_attackBox->Collider3D()->SetOffsetPos(Vec3(0.f, 30.f, -120.f));
				m_attackBox->Collider3D()->SetOffsetScale(Vec3(1.5f, 1.f, 1.5f));
				m_attackBox->Collider3D()->SetSphere(false);
				((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDamage(damage);
				((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraPercent(extraSkillDamage);
				((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraFlat(skillAmp);
				((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDebuff(DEBUFF_TYPE::SLOW);
				((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDebuffTime(2.f);
				((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDebuffRate(0.4f);
				((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetHitSound(m_skillQ_Hit_Sound.Get());
			}
		}

		if (Animator3D()->GetCurFrameIndex() == 15 && nullptr != m_attackBox && m_attackBox->IsOn())
		{
			m_attackBox->SetObjOff();
		}

		if (Animator3D()->IsAnimFinish())
		{
			m_bSkill_Q_Motion = false;
			m_skillQ_Effect->SetObjOff();
			SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::WAIT, false);
		}
	}

	if (m_bAssaultMotion)
	{
		if (m_bAssaultStartMotion)
		{
			//현재 충돌체 구조를 수정할 예정이기에 예시로 제작
			// 현재 처음 충돌을 하게 되는 경우 해당 애니메이션을 loop로 변경을 해주고,
			// 돌진하는 위치도 다시 세팅을 해준다.

			if (nullptr != m_attackBox && ((CHitBoxScript*)m_attackBox->GetScripts()[0])->IsCharacterFirstCol())
			{
				// 처음 충돌 했을 경우 지금까지 돌진 거리 초기화, 모션 변경

				m_bAssaultStartMotion = false;
				m_bAssaultLoopMotion = true;
				m_curAssaultDistance = 0.f;
				SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::SKILL03_LOOP, false);
				if (nullptr != m_skillE_Slide_Sound.Get())
					m_skillE_Slide_Sound->Stop();
			}

			Vec3 pos = Transform()->GetWorldPos();
			pos += (m_assaultSpeed * m_assaultDir * DELTATIME);
			m_curAssaultDistance += m_assaultSpeed * DELTATIME;

			if (m_naviMesh->NaviMesh()->IsAboveTile(Object(), m_assaultDir))
			{
				Transform()->SetLocalPos(pos);
			}

			//아무와도 충돌하지 않은 경우
			if (Animator3D()->IsAnimFinish())
			{
				m_bAssaultStartMotion = false;
				m_bAssaultMotion = false;
				m_bSkill_E_Motion = false;

				if (nullptr != m_attackBox)
					m_attackBox->SetObjOff();
			}

		}
		else
		{
			if (m_bAssaultLoopMotion)
			{
				if (Animator3D()->IsAnimFinish())
				{
					m_bAssaultLoopMotion = false;
					m_bAssaultEndMotion = true;

					if (nullptr != m_attackBox)
						m_attackBox->SetObjOff();

					SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::SKILL03_END, false);
				}
			}
			//돌진시 끝나는 모션인지 확인
			else if (m_bAssaultEndMotion)
			{
				if (Animator3D()->IsAnimFinish())
				{
					m_bAssaultMotion = false;
					m_bSkill_E_Motion = false;
					SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::WAIT, false);

					if (nullptr != m_attackBox)
						m_attackBox->SetObjOff();
				}
			}
		}
	}

	Attack();

}

void CHyunwooScript::lateUpdate()
{
	CPlayerScript::lateUpdate();
}

void CHyunwooScript::OnColliderEnter(CCollider3D* _other)
{
}

void CHyunwooScript::OnCollider(CCollider3D* _other)
{
}

void CHyunwooScript::OnColliderExit(CCollider3D* _other)
{
}

void CHyunwooScript::Attack()
{
	CPlayerScript::Attack();

	if (m_bTargetAttack && !m_bPassiveStackUp)
	{
		m_bPassiveStackUp = m_bTargetAttack;
		++m_passiveStack;
	}

	if (m_bPassiveStackUp && !m_bTargetAttack)
	{
		m_bPassiveStackUp = false;
	}

	if (m_passiveStack >= m_passiveMaxStack)
		PassiveActivation();
}

void CHyunwooScript::PassiveActivation()
{
	m_passiveStack = 0;

	m_curHP += m_maxHP * (0.04f * (float)m_passive_LV);

	if (m_curHP > m_maxHP)
		m_curHP = m_maxHP;
}

void CHyunwooScript::SetAnimAction(PLAYER_ANIMTYPE _action, UINT _AnimType, bool _bLoop, float _animSpeed, UINT _count)
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

		switch ((HYUNWOO_ANIM)_AnimType)
		{
		case HYUNWOO_ANIM::RUN:
			animName += L"run";
			break;
		case HYUNWOO_ANIM::WAIT:
			animName += L"wait";
			break;
		case HYUNWOO_ANIM::ATK01:
			animName += L"atk01";
			break;
		case HYUNWOO_ANIM::ATK02:
			animName += L"atk02";
			break;
		case HYUNWOO_ANIM::SKILL01:
			animName += L"skill01";
			break;
		case HYUNWOO_ANIM::SKILL02:
			animName += L"skill02";
			break;
		case HYUNWOO_ANIM::SKILL03_START:
			animName += L"skill03_start";
			break;
		case HYUNWOO_ANIM::SKILL03_LOOP:
			animName += L"skill03_loop";
			break;
		case HYUNWOO_ANIM::SKILL03_END:
			animName += L"skill03_end";
			break;
		case HYUNWOO_ANIM::SKILL04_START:
			animName += L"skill04_start";
			break;
		case HYUNWOO_ANIM::SKILL04_END:
			animName += L"skill04_end";
			break;
		case HYUNWOO_ANIM::SKILL04_LOOP:
			animName += L"skill04_loop";
			break;
		case HYUNWOO_ANIM::SKILL04_CC_END:
			animName += L"skill04_CC_end";
			break;
		case HYUNWOO_ANIM::WEAPONSKILL:
			animName += L"Weaponskill";
			break;
		case HYUNWOO_ANIM::WEAPONSKILL_2:
			animName += L"Weaponskill02";
			break;
		case HYUNWOO_ANIM::END:
			return;
		default:
			return;
		}

		Animator3D()->Play(animName, _bLoop, _animSpeed, _count);
		m_animType = _action;
		m_curAnim = _AnimType;
	}
}

void CHyunwooScript::StatsReSet()
{
	CPlayerScript::StatsReSet();

	m_offense = 38.f;
	m_defense = 23.f;
	m_maxHP = 670.f;
	m_curHP = m_maxHP;
	m_maxStamina = 350.f;
	m_curStamina = m_maxStamina;
	m_moveSpeed = 3.45f;
	m_attackSpeed = 0.14f;
	m_healthRegen = 0.8f;
	m_staminaRegen = 1.8f;
	m_criticalChance = 0.f;
	m_FOV = 8.f;			//시야의 경우 인 게임에서 확인 후 지정
	m_attackRange = 195.f;
}

void CHyunwooScript::LvUp()
{
	CPlayerScript::LvUp();

	m_offense += 3.4f;
	m_defense += 1.9f;
	m_maxHP += 76.f;
	m_curHP += 76.f;
	m_maxStamina += 16.f;
	m_curStamina += 16.f;
	m_healthRegen += 0.04f;
	m_staminaRegen += 0.04f;
}


void CHyunwooScript::Skill_Q(CGameObject* pObj)
{
	m_curStamina -= m_skill_Q_SM;
	// 쿨 타임 감소 아이템까지 적용할 시 뒤에 수식 추가하면 됨
	m_skill_Q_CT = 9.5f - float(m_skill_Q_Lv);
	m_skill_Q_MaxCT = m_skill_Q_CT;


	//Q 스킬 애니메이션 적용, 애니메이션 중에 이동 불가능하게 설정하기 위해 bool 값 넣음
	m_bSkill_Q_Motion = true;

	m_assaultDir = -Transform()->GetWorldDir(DIR_TYPE::FRONT);

	SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::SKILL01, false);

	if (nullptr != m_skillQ_Attack_Sound.Get() && IsPlaySound())
		m_skillQ_Attack_Sound->Play(1, 0.1f);

	m_skillVoiceSound = CResourceManager::GetInst()->FindRes<CSound>(L"Hyunwoo_PlaySkillVoice_Q");
	if (nullptr != m_skillVoiceSound.Get() && IsPlaySound())
		m_skillVoiceSound->Play(1, 0.1f);

}

void CHyunwooScript::Skill_W(CGameObject* pObj)
{
	m_curStamina -= m_skill_W_SM;
	m_skill_W_CT = 32.f - float(m_skill_W_Lv * 4);


	m_skill_W_MaxCT = m_skill_W_CT;
	//2.5초 동안 방어력 증가
	//즉시 시전
	// 방해 효과 무시 1초
	
	if (nullptr != m_skillW_Sound.Get() && IsPlaySound())
		m_skillW_Sound->Play(1, 0.1f);

	if (nullptr != m_skillW_Effect)
	{
		m_skillW_Effect->SetObjOn();
		CEffectScript* effectScript = (CEffectScript*)m_skillW_Effect->GetScripts()[0];
		effectScript->Reset();
	}

	// 방어력 증가를 스킬에 따른 방어력 증가로 할 것인지 아니면 직접 더해줄 것인지
	m_defense_Buff = float((m_skill_W_Lv - 1) * 14) + m_defense * 0.1f;
	m_skill_W_CumTime = 0.f;
	m_bBuff = true;
}

void CHyunwooScript::Skill_E(CGameObject* pObj)
{
	m_curStamina -= m_skill_E_SM;
	m_skill_E_CT = 19.f - float(m_skill_E_Lv);
	m_skill_E_MaxCT = m_skill_E_CT;

	m_assaultDir = -Transform()->GetWorldDir(DIR_TYPE::FRONT);

	m_assaultStratPos = Transform()->GetWorldPos();
	m_curAssaultDistance = 0.f;
	m_bAssaultMotion = true;
	m_bSkill_E_Motion = true;

	SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::SKILL03_START, false, 1.f, 3);
	m_bAssaultStartMotion = true;

	if (nullptr != m_skillE_Slide_Sound.Get() && IsPlaySound())
		m_skillE_Slide_Sound->Play(1, 0.1f);

	//어택 박스 생성
	m_attackBox->SetOnOff(true);

	// 어택 박스 충돌 시 데미지 부여
	float damage = m_finalAttackPower * 0.55f;
	float extraSkillDamage = m_ExtraSkillDamage + (m_ExtraSkillDamagePerLevel * m_level);
	float skillAmp = m_SkillAmplification + (m_SkillAmplificationPerLevel * m_level);
	m_attackBox->Collider3D()->SetOffsetPos(Vec3(0.f, 98.2f, -40.f));
	m_attackBox->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
	m_attackBox->Collider3D()->SetSphere(false);
	((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetDamage(damage);
	((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraPercent(extraSkillDamage);
	((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetExtraFlat(skillAmp);
	((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetRate(true);
	((CHitBoxScript*)m_attackBox->GetScripts()[0])->SetHitSound(m_skillE_Hit_Sound.Get());
	((CHitBoxScript*)m_attackBox->GetScripts()[0])->Reset();
}

void CHyunwooScript::Skill_R(CGameObject* pObj)
{
	if (!m_bCharge)
	{
		m_bSkill_R_Motion = true;
		m_curStamina -= m_skill_R_SM;
		m_bCharge = true;
		m_bChargeStartMotion = true;
		m_chargeTime = 0.f;
		m_chargeRadio = 0.01f;

		if (nullptr != m_skill_R_RangeLine)
		{
			m_skill_R_RangeLine->SetObjOn();

			m_skill_R_Range->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &m_chargeRadio);
			m_skill_R_Range->SetObjOn();
		}

		if (nullptr != m_skillR_Charging_Sound.Get() && IsPlaySound())
			m_skillR_Charging_Sound->Play(1, 0.1f);

		SetAnimAction(PLAYER_ANIMTYPE::CHARACTER_ANIM, (UINT)HYUNWOO_ANIM::SKILL04_START, false);
		// 차징 애니메이션 시작
	}
}

void CHyunwooScript::SaveToScene(FILE* _file)
{
	CPlayerScript::SaveToScene(_file);
}

void CHyunwooScript::LoadFromScene(FILE* _file)
{
	CPlayerScript::LoadFromScene(_file);
}
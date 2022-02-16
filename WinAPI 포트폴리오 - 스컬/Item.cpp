#include "stdafx.h"
#include "Item.h"
#include "Player.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "SceneManager.h"

Item::Item() :
	m_owner(nullptr),
	m_itemType(SUBJECT_TYPE::ITEM),
	m_itemStyle(ITEM_TYPE::NONE),
	m_itemGrade(GRADE::NORMAL),
	m_arrayEngrave{},
	m_bAdapt(false),
	m_bAdaptSkil(false),
	m_bSoar(false),
	m_duration(0.f),
	m_coolTime(40.f)
{
	for (int i = 0; i < 2; ++i)
	{
		m_arrayEngrave[i] = ENGRAVE_TYPE::NONE;
	}
}

Item::~Item()
{
}

void Item::update()
{
	if (SUBJECT_TYPE::ITEM == m_itemType)
	{
		AdaptItem();
		AdaptEngrave();
	}

	if (SUBJECT_TYPE::ESSENCE == m_itemType)
	{
		PassiveEssense();
		UseEssence();
	}
}

void Item::lateUpdate()
{
}

void Item::AdaptItem()
{
	if (!m_bAdapt)
	{
		if (ITEM_TYPE::ADMISSION_NOTICE_SEAT == m_itemStyle)
		{
			m_arrayEngrave[0] = ENGRAVE_TYPE::BRAVE;
			m_arrayEngrave[1] = ENGRAVE_TYPE::HEART;

			int curPhysicalDamage = m_owner->GetPhysicalAtk();
			int curMagicalDamage = m_owner->GetMagicalAtk();

			curPhysicalDamage += int((float)curPhysicalDamage * 0.1f);
			curMagicalDamage += int((float)curMagicalDamage * 0.1f);

			m_owner->SetPhysicalAtk(curPhysicalDamage);
			m_owner->SetMagicalAtk(curMagicalDamage);

			m_bAdapt = true;
		}

		if (ITEM_TYPE::RING_OF_WIND == m_itemStyle)
		{
			m_arrayEngrave[0] = ENGRAVE_TYPE::SCAMPER;
			m_arrayEngrave[1] = ENGRAVE_TYPE::SOAR;

			float attackSpeed = m_owner->GetAttackSpeed();
			float speed = m_owner->GetSpeedRate();

			attackSpeed *= 1.15f;
			speed *= 1.1f;

			m_owner->SetAttackSpeed(attackSpeed);
			m_owner->SetSpeedRate(speed);

			m_bAdapt = true;
		}
	}
}

void Item::AdaptEngrave()
{
	for (int i = 0; i < 2; ++i)
	{
		switch (m_arrayEngrave[i])
		{
		case ENGRAVE_TYPE::ENDURANCE :
		{

		}
			break;
		case ENGRAVE_TYPE::ADAPTATION:
		{
		}
			break;
		case ENGRAVE_TYPE::REMAINS:
			break;
		case ENGRAVE_TYPE::SCAMPER:
		{
			if (!m_bAdaptSkil)
			{
				float attackSpeed = m_owner->GetAttackSpeed();
				float speed = m_owner->GetSpeedRate();

				attackSpeed *= 1.1f;
				speed *= 1.1f;

				m_owner->SetAttackSpeed(attackSpeed);
				m_owner->SetSpeedRate(speed);

				m_bAdaptSkil = true;
			}
		}
			break;
		case ENGRAVE_TYPE::SOAR:
		{
			if (m_owner->IsJump())
			{
				float physicalAttack = (float)m_owner->GetPhysicalAtk();
				float magicalAttack = (float)m_owner->GetMagicalAtk();

				physicalAttack *= 1.35f;
				magicalAttack *= 1.35f;

				m_owner->SetPhysicalAtk((int)physicalAttack);
				m_owner->SetMagicalAtk((int)magicalAttack);
				m_bSoar = true;
			}
			else
			{
				if (m_bSoar)
				{
					m_duration += DELTATIME;

					if (m_duration > 1.f)
					{
						float physicalAttack = (float)m_owner->GetPhysicalAtk();
						float magicalAttack = (float)m_owner->GetMagicalAtk();

						physicalAttack /= 1.35f;
						magicalAttack /= 1.35f;

						m_owner->SetPhysicalAtk((int)physicalAttack);
						m_owner->SetMagicalAtk((int)magicalAttack);

						m_duration = 0.f;
						m_bSoar = false;
					}
				}
			}
		}
			break;
		case ENGRAVE_TYPE::RAID:
			break;
		case ENGRAVE_TYPE::MUTATION:
			break;
		case ENGRAVE_TYPE::HEART:
		{
			if (!m_bAdaptSkil)
			{
				int maxHP = m_owner->GetMaxHP();
				float hpRatio = m_owner->GetHPRatio();
				maxHP += 50;

				int curHP = int(hpRatio * float(maxHP));
				m_owner->SetMaxHP(maxHP);
				m_owner->SetCurHP(curHP);
			}
		}
			break;
		case ENGRAVE_TYPE::BRAVE:
		{
			if (!m_bAdaptSkil)
			{
				int damage = m_owner->GetPhysicalAtk();
				damage += int((float)damage * 0.5f);

				m_owner->SetPhysicalAtk(damage);
			}
		}
			break;
		case ENGRAVE_TYPE::FIGHT:
			break;
		case ENGRAVE_TYPE::LUNACY:
			break;
		case ENGRAVE_TYPE::COLLAPSE:
			break;
		}
	}

	m_bAdaptSkil = true;
}

void Item::UseEssence()
{
	if (KEY_TAP(KEY::D) && !m_bAdaptSkil)
	{
		m_owner->SetDamageReduction(0.4f);
		m_duration = 10.f;
		m_bAdaptSkil = true;
	}

	if (m_bAdaptSkil)
	{
		m_coolTime -= DELTATIME;

		if (m_coolTime <= 0)
		{
			m_coolTime = 40.f;
			m_bAdaptSkil = false;
		}
	}
	
	m_duration -= DELTATIME;
	if (m_duration <= 0)
	{
		m_owner->SetDamageReduction(0.f);
	}
}

void Item::PassiveEssense()
{
	if (!m_bAdapt)
	{
		int maxHP = m_owner->GetMaxHP();
		float hpRatio = m_owner->GetHPRatio();
		maxHP += 40;
		int curHP = int(hpRatio * float(maxHP));
		m_owner->SetMaxHP(maxHP);
		m_owner->SetCurHP(curHP);
		m_bAdapt = true;
	}
}

void Item::DestroyItem(ITEM_TYPE _type)
{
	if (ITEM_TYPE::ADMISSION_NOTICE_SEAT == m_itemStyle)
	{
		int curPhysicalDamage = m_owner->GetPhysicalAtk();
		int curMagicalDamage = m_owner->GetMagicalAtk();

		curPhysicalDamage -= int((float)curPhysicalDamage * 0.1f);
		curMagicalDamage -= int((float)curMagicalDamage * 0.1f);

		m_owner->SetPhysicalAtk(curPhysicalDamage);
		m_owner->SetMagicalAtk(curMagicalDamage);

		ReleaseEngrave();
	}

	if (ITEM_TYPE::RING_OF_WIND == m_itemStyle)
	{
		float attackSpeed = m_owner->GetAttackSpeed();
		float speed = m_owner->GetSpeedRate();

		attackSpeed /= 1.15f;
		speed /= 1.1f;

		m_owner->SetAttackSpeed(attackSpeed);
		m_owner->SetSpeedRate(speed);
		ReleaseEngrave();
	}
}

void Item::ReleaseEngrave()
{
	for (int i = 0; i < 2; ++i)
	{
		switch (m_arrayEngrave[i])
		{
		case ENGRAVE_TYPE::ENDURANCE:
		{

		}
			break;
		case ENGRAVE_TYPE::ADAPTATION:
		{
		}
			break;
		case ENGRAVE_TYPE::REMAINS:
			break;
		case ENGRAVE_TYPE::SCAMPER:
		{
			float attackSpeed = m_owner->GetAttackSpeed();
			float speed = m_owner->GetSpeedRate();

			attackSpeed /= 1.1f;
			speed /= 1.1f;

			m_owner->SetAttackSpeed(attackSpeed);
			m_owner->SetSpeedRate(speed);
		}
			break;
		case ENGRAVE_TYPE::RAID:
			break;
		case ENGRAVE_TYPE::MUTATION:
			break;
		case ENGRAVE_TYPE::HEART:
		{
			int maxHP = m_owner->GetMaxHP();
			float hpRatio = m_owner->GetHPRatio();
			maxHP -= 50;

			int curHP = int(hpRatio * float(maxHP));
			m_owner->SetMaxHP(maxHP);
			m_owner->SetCurHP(curHP);
		}
			break;
		case ENGRAVE_TYPE::BRAVE:
		{
			int damage = m_owner->GetPhysicalAtk();
			damage -= int((float)damage * 0.5f);

			m_owner->SetPhysicalAtk(damage);
		}
			break;
		case ENGRAVE_TYPE::FIGHT:
			break;
		case ENGRAVE_TYPE::LUNACY:
			break;
		case ENGRAVE_TYPE::COLLAPSE:
			break;
		}
	}
}
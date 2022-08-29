// Fill out your copyright notice in the Description page of Project Settings.


#include "StructureManager.h"


UStructureManager* UStructureManager::m_This = nullptr;
UWorld* UStructureManager::m_World = nullptr;

UStructureManager::UStructureManager()
{
}

UStructureManager::~UStructureManager()
{
}

UStructureManager* UStructureManager::GetInst(UWorld* _world)
{
	if (nullptr == m_This)
	{
		m_This = NewObject<UStructureManager>();
		m_This->AddToRoot();

		m_World = _world;
	}

	if (m_World != _world)
	{
		m_World = _world;
	}

	return m_This;
}

void UStructureManager::Destroy()
{
	if (nullptr != m_This)
	{
		m_This->ConditionalBeginDestroy();
		m_This = nullptr;
		m_World = nullptr;
	}
}

void UStructureManager::Init()
{
	for (int i = 0; i < (int)ESTRUCTURE_TYPE::END; ++i)
	{
		m_arrStructureLv[i] = 0;

		switch ((ESTRUCTURE_TYPE)i)
		{
		case ESTRUCTURE_TYPE::BARRIER:
			m_arrInfo[i].ClassName = TEXT("Barrier");
			m_arrInfo[i].BaseGold = 50;
			m_arrInfo[i].PerLevelGold = 10;
			m_arrInfo[i].BaseHP = 250.f;
			m_arrInfo[i].PerLevelHP = 80.f;
			m_arrInfo[i].BaseAP = 0.f;
			m_arrInfo[i].PerLevelAP = 0.f;
			m_arrInfo[i].TurnSpeed = 0.f;
			m_arrInfo[i].ReloadTime = 0.f;
			break;
		case ESTRUCTURE_TYPE::TURRET_A:
			m_arrInfo[i].ClassName = TEXT("Turret_A");
			m_arrInfo[i].BaseGold = 100;
			m_arrInfo[i].PerLevelGold = 20;
			m_arrInfo[i].BaseHP = 120.f;
			m_arrInfo[i].PerLevelHP = 20.f;
			m_arrInfo[i].BaseAP = 15.f;
			m_arrInfo[i].PerLevelAP = 2.f;
			m_arrInfo[i].TurnSpeed = 3.5f;
			m_arrInfo[i].ReloadTime = 2.f;
			m_arrInfo[i].AttackRange = 3000.f;
			break;
		case ESTRUCTURE_TYPE::TURRET_B:
			m_arrInfo[i].ClassName = TEXT("Turret_B");
			m_arrInfo[i].BaseGold = 200;
			m_arrInfo[i].PerLevelGold = 50;
			m_arrInfo[i].BaseHP = 350.f;
			m_arrInfo[i].PerLevelHP = 10.f;
			m_arrInfo[i].BaseAP = 40.f;
			m_arrInfo[i].PerLevelAP = 5.f;
			m_arrInfo[i].TurnSpeed = 1.5f;
			m_arrInfo[i].ReloadTime = 5.f;
			m_arrInfo[i].AttackRange = 3000.f;
			break;
		case ESTRUCTURE_TYPE::TURRET_C:
			m_arrInfo[i].ClassName = TEXT("Turret_C");
			m_arrInfo[i].BaseGold = 160;
			m_arrInfo[i].PerLevelGold = 40;
			m_arrInfo[i].BaseHP = 250.f;
			m_arrInfo[i].PerLevelHP = 30.f;
			m_arrInfo[i].BaseAP = 35.f;
			m_arrInfo[i].PerLevelAP = 8.f;
			m_arrInfo[i].TurnSpeed = 2.f;
			m_arrInfo[i].ReloadTime = 4.f;
			m_arrInfo[i].AttackRange = 3000.f;
			break;
		case ESTRUCTURE_TYPE::TURRET_D:
			m_arrInfo[i].ClassName = TEXT("Turret_D");
			m_arrInfo[i].BaseGold = 130;
			m_arrInfo[i].PerLevelGold = 15;
			m_arrInfo[i].BaseHP = 150.f;
			m_arrInfo[i].PerLevelHP = 10.f;
			m_arrInfo[i].BaseAP = 5.f;
			m_arrInfo[i].PerLevelAP = 3.f;
			m_arrInfo[i].TurnSpeed = 4.f;
			m_arrInfo[i].ReloadTime = 1.f;
			m_arrInfo[i].AttackRange = 3000.f;
			break;
		case ESTRUCTURE_TYPE::NEXUS:
			m_arrInfo[i].ClassName = TEXT("Nexus");
			m_arrInfo[i].MaxHP = 2000.f;
			m_arrInfo[i].CurHP = m_arrInfo[i].MaxHP;
			break;
		}

		CalStat((ESTRUCTURE_TYPE)i);
	}

	m_MaxLv = 5;
}

void UStructureManager::Reset()
{
	for (int i = 0; i < (int)ESTRUCTURE_TYPE::END; ++i)
	{
		m_arrStructureLv[i] = 0;
	}
}

void UStructureManager::CalStat(ESTRUCTURE_TYPE _type)
{
	switch (_type)
	{
	case ESTRUCTURE_TYPE::BARRIER:
		m_arrInfo[(int)_type].MaxHP = m_arrInfo[(int)_type].BaseHP + m_arrInfo[(int)_type].PerLevelHP * m_arrStructureLv[(int)_type];
		m_arrInfo[(int)_type].CurHP = m_arrInfo[(int)_type].MaxHP;
		break;
	case ESTRUCTURE_TYPE::TURRET_A:
	case ESTRUCTURE_TYPE::TURRET_B:
	case ESTRUCTURE_TYPE::TURRET_C:
	case ESTRUCTURE_TYPE::TURRET_D:
		m_arrInfo[(int)_type].MaxHP = m_arrInfo[(int)_type].BaseHP + m_arrInfo[(int)_type].PerLevelHP * m_arrStructureLv[(int)_type];
		m_arrInfo[(int)_type].AttackPower = m_arrInfo[(int)_type].BaseAP + m_arrInfo[(int)_type].PerLevelAP * m_arrStructureLv[(int)_type];
		m_arrInfo[(int)_type].CurHP = m_arrInfo[(int)_type].MaxHP;
		break;
	}
}

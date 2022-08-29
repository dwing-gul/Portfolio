// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGameInstance.h"
#include "Manager/SkillManager.h"
#include "Manager/StructureManager.h"
#include "Manager/EffectManager.h"
#include "Manager/EnemyManager.h"

UProjectGameInstance::UProjectGameInstance() :
	m_SelectClassType(ECLASS_TYPE::MURDOCK)
{
	USkillManager::GetInst(GetWorld())->Init();
	UStructureManager::GetInst(GetWorld())->Init();

	ConstructorHelpers::FObjectFinder<UDataTable> PlayerTable(TEXT("DataTable'/Game/Blueprint/Player/PlayerInfoTable.PlayerInfoTable'"));
	if (PlayerTable.Succeeded())
	{
		m_PlayerTable = PlayerTable.Object;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> EnemyTable(TEXT("DataTable'/Game/Blueprint/Enemy/EnemyInfoTable.EnemyInfoTable'"));
	if (EnemyTable.Succeeded())
	{
		m_EnemyTable = EnemyTable.Object;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> SummonsTable(TEXT("DataTable'/Game/Blueprint/Summons/SummonsInfoTable.SummonsInfoTable'"));
	if (SummonsTable.Succeeded())
	{
		m_SummonsTable = SummonsTable.Object;
	}
}

UProjectGameInstance::~UProjectGameInstance()
{
	USkillManager::Destroy();
	UStructureManager::Destroy();
	UEffectManager::Destroy();
	UEnemyManager::Destroy();
}

const FPlayerInfo* UProjectGameInstance::GetPlayerInfo(const FString& _RowName)
{
	FPlayerInfo* PlayerInfo = m_PlayerTable->FindRow<FPlayerInfo>(FName(_RowName), TEXT(""));
	return PlayerInfo;
}

const FEnemyInfo* UProjectGameInstance::GetEnemyInfo(const FString& _RowName)
{
	FEnemyInfo* EnemyInfo = m_EnemyTable->FindRow<FEnemyInfo>(FName(_RowName), TEXT(""));
	return EnemyInfo;
}

const FSummonsInfo* UProjectGameInstance::GetSummonsInfo(const FString& _RowName)
{
	FSummonsInfo* SummonsInfo = m_SummonsTable->FindRow<FSummonsInfo>(FName(_RowName), TEXT(""));
	return SummonsInfo;
}

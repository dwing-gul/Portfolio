// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "global.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private :
	UDataTable* m_PlayerTable;
	UDataTable* m_EnemyTable;
	UDataTable* m_SummonsTable;
	UDataTable* m_StructureTable;
	ECLASS_TYPE m_SelectClassType;

public :
	const FPlayerInfo* GetPlayerInfo(const FString& _RowName);
	const FEnemyInfo* GetEnemyInfo(const FString& _RowName);
	const FSummonsInfo* GetSummonsInfo(const FString& _RowName);
	void SetSelectClassType(ECLASS_TYPE _Type) { m_SelectClassType = _Type; }
	ECLASS_TYPE GetSelectClassType() { return m_SelectClassType; }
	
public :
	UProjectGameInstance();
	~UProjectGameInstance();
	
};

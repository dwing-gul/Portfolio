// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyManager.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UEnemyManager : public UObject
{
	GENERATED_BODY()
	
private:
	UEnemyManager();
	~UEnemyManager();

private:
	static UEnemyManager* m_This;
	static UWorld* m_World;

public:
	static UEnemyManager* GetInst(UWorld* _world);
	static void Destroy();
	
};

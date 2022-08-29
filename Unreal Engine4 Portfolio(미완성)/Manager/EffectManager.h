// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EffectManager.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UEffectManager : public UObject
{
	GENERATED_BODY()
	
private:
	UEffectManager();
	~UEffectManager();

private:
	static UEffectManager* m_This;
	static UWorld* m_World;

public:
	static UEffectManager* GetInst(UWorld* _world);
	static void Destroy();

public :
	void CreateEffect(EEFFECT_TYPE _type, FTransform _trans, ULevel* _level);
};

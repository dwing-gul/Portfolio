// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "global.h"
#include <Blueprint/UserWidget.h>
#include "UI/MainHUDWidget.h"
#include "UI/PlayerHUDWidget.h"
#include "UI/SummonsHUDWidget.h"
#include "UI/NexusHPWidget.h"
#include "UI/SkillHUDWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API AUnrealProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
private :
	TSubclassOf<UUserWidget> m_MainHUDClass;
	UMainHUDWidget* m_MainHUD;

public :
	UMainHUDWidget* GetMainHUD() { return m_MainHUD; }
	void UpdatePlayerHUD(float _CurHPRatio, float _CurEPRatio, float _CurShieldRatio);
	void UpdateSummonHUD(float _CurHPRatio);
	void UpdateNexusHUD(float _CurHPRatio);
	void UpdateQSkillHUD();
	void UpdateESkillHUD();
	void UpdateRSkillHUD();
	
public :
	AUnrealProjectGameModeBase();
	~AUnrealProjectGameModeBase();

public :
	virtual void BeginPlay() override;
};

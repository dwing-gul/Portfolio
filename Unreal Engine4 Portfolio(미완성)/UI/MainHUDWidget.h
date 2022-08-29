// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include <Components/Button.h>
#include "PreBattleHUDWidget.h"
#include "PlayerHUDWidget.h"
#include "SkillHUDWidget.h"
#include "PauseHUDWidget.h"
#include "NexusHPWidget.h"
#include "SummonsHUDWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
private :
	UPreBattleHUDWidget* m_PreBattleHUD;
	UPlayerHUDWidget* m_PlayerHUD;
	USummonsHUDWidget* m_SummonsHUD;
	UNexusHPWidget* m_NexusHUD;
	USkillHUDWidget* m_Q_SkillHUD;
	USkillHUDWidget* m_E_SkillHUD;
	USkillHUDWidget* m_R_SkillHUD;
	UButton* m_Pause;
	UPauseHUDWidget* m_PauseHUD;

public :
	UPlayerHUDWidget* GetPlayerHUD() { return m_PlayerHUD; }
	USummonsHUDWidget* GetSummonsHUD() { return m_SummonsHUD; }
	UNexusHPWidget* GetNexusHUD() { return m_NexusHUD; }
	USkillHUDWidget* GetQSkillHUD() { return m_Q_SkillHUD; }
	USkillHUDWidget* GetESkillHUD() { return m_E_SkillHUD; }
	USkillHUDWidget* GetRSkillHUD() { return m_R_SkillHUD; }
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public :
	UFUNCTION()
	void PauseOnClicked();
};

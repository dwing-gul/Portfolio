// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include "SlotHUDWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UUpgradeHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
private :
	ESKILL m_SelectSkill;

	UTextBlock* m_Power;
	UTextBlock* m_Speed;
	UTextBlock* m_Intellect;
	UTextBlock* m_Health;
	UTextBlock* m_Mental;
	UTextBlock* m_Point;
	UTextBlock* m_Gold;

	UTextBlock* m_StimLv;
	UTextBlock* m_DroneLv;
	UTextBlock* m_ShieldLv;
	UTextBlock* m_BeamLv;
	UTextBlock* m_GigantLv;

	UTextBlock* m_BarrierLv;
	UTextBlock* m_TurretALv;
	UTextBlock* m_TurretBLv;
	UTextBlock* m_TurretCLv;
	UTextBlock* m_TurretDLv;

	UButton* m_PowerPlus;
	UButton* m_SpeedPlus;
	UButton* m_IntellectPlus;
	UButton* m_HealthPlus;
	UButton* m_MentalPlus;
	UButton* m_StimpackIcon;
	UButton* m_DroneBombIcon;
	UButton* m_ShieldIcon;
	UButton* m_BeamIcon;
	UButton* m_GigantIcon;
	UButton* m_Stimpack;
	UButton* m_DroneBomb;
	UButton* m_Shield;
	UButton* m_Beam;
	UButton* m_Gigant;
	UButton* m_Barrier;
	UButton* m_Turret_A;
	UButton* m_Turret_B;
	UButton* m_Turret_C;
	UButton* m_Turret_D;

	USlotHUDWidget* m_QSkill;
	USlotHUDWidget* m_ESkill;
	USlotHUDWidget* m_RSkill;

	UTexture2D* m_Blue;
	UTexture2D* m_White;

public :
	UFUNCTION()
	void PowerOnClicked();

	UFUNCTION()
	void SpeedOnClicked();

	UFUNCTION()
	void IntellectOnClicked();

	UFUNCTION()
	void HealthOnClicked();

	UFUNCTION()
	void MentalOnClicked();

	UFUNCTION()
	void StimpackOnClicked();

	UFUNCTION()
	void DroneBombOnClicked();

	UFUNCTION()
	void ShieldOnClicked();

	UFUNCTION()
	void BeamOnClicked();

	UFUNCTION()
	void GigantOnClicked();

	UFUNCTION()
	void StimpackIconOnClicked();

	UFUNCTION()
	void DroneBombIconOnClicked();

	UFUNCTION()
	void ShieldIconOnClicked();

	UFUNCTION()
	void BeamIconOnClicked();

	UFUNCTION()
	void GigantIconOnClicked();

	UFUNCTION()
	void BarrierOnClicked();

	UFUNCTION()
	void Turret_AOnClicked();

	UFUNCTION()
	void Turret_BOnClicked();

	UFUNCTION()
	void Turret_COnClicked();

	UFUNCTION()
	void Turret_DOnClicked();
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public :
	void SkillLvUp(ESKILL _skill);
	void StructureLvUp(ESTRUCTURE_TYPE _type, UTextBlock* _target, const FName& _name);
};

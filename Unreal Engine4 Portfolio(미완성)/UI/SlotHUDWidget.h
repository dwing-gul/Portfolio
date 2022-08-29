// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../global.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USlotHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
private :
	UButton* m_Slot;
	UImage* m_SkillImage;
	UTextBlock* m_KeyName;
	ESKILL m_CurSkill;
	ESKILL_KEY m_Key;

public :
	UFUNCTION()
	void SlotOnClicked();

	void SetCurSkill(ESKILL _Skill) { m_CurSkill = _Skill; }
	void SetKey(ESKILL_KEY _Key) { m_Key = _Key; }
	void SetKeyName(const FString& _Name);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

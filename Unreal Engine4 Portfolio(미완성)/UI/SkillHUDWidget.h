// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USkillHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
private :
	UImage* m_SkillImage;
	UTextBlock* m_SkillKey;

public :
	void SetText(const FString& _Key);
	void SetMaterial(UMaterialInstance* _MaterialInstance) { m_SkillImage->SetBrushFromMaterial(_MaterialInstance); }

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

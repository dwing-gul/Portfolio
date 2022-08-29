// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include <Components/ProgressBar.h>
#include <Components/Image.h>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SummonsHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API USummonsHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UProgressBar* m_HPBar;
	UImage* m_Portrait;

public:
	void SetHP(float _Ratio) { m_HPBar->SetPercent(_Ratio); }
	void SetMaterial(UMaterialInstance* _Material) { m_Portrait->SetBrushFromMaterial(_Material); }
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

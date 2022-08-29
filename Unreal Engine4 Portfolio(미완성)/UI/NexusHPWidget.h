// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NexusHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UNexusHPWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UProgressBar* m_HPBar;
	UTextBlock* m_HPRatio;

public :
	void SetHP(float _Ratio) { m_HPBar->SetPercent(_Ratio); }
	void SetText(const FString& _Ratio);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

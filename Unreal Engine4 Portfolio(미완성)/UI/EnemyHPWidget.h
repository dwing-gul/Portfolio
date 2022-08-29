// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include <Components/ProgressBar.h>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UEnemyHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UProgressBar* m_HPBar;

public:
	void SetHP(float _Ratio) { m_HPBar->SetPercent(_Ratio); }

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

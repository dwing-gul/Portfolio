// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyInfoHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UEnemyInfoHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};

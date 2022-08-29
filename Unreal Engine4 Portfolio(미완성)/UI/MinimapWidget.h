// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include <Components/Image.h>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()
	
private :
	UImage* m_Player;
	UImage* m_Nexus;
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

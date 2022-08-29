// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include <Components/Button.h>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PreBattleHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UPreBattleHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
private :
	UButton* m_Compelete;
	UButton* m_Back;

public :
	UFUNCTION()
	void CompeleteOnClicked();

	UFUNCTION()
	void BackOnClicked();

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

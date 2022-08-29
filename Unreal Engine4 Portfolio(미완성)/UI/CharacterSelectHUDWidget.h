// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../global.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include "../SelectCharacter/SelectCharacter.h"
#include "SelectInfoHUDWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJECT_API UCharacterSelectHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
private :
	ASelectCharacter* m_SelectMurdock;
	ASelectCharacter* m_SelectBelica;
	ASelectCharacter* m_SelectTwinblast;
	UImage* m_Background;
	USelectInfoHUDWidget* m_SelectInfo;
	UButton* m_Compelete;
	UButton* m_Murdock;
	UButton* m_LtBelica;
	UButton* m_Twinblast;

public :
	UFUNCTION()
	void CompeleteOnClicked();

	UFUNCTION()
	void MurdockOnClicked();

	UFUNCTION()
	void LtBelicaOnClicked();

	UFUNCTION()
	void TwinblastOnClicked();
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
